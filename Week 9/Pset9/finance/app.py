import os
from datetime import datetime

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    
    # Query database for user's symbols and shares
    portfolio = db.execute("SELECT symbol, SUM(shares) FROM transactions WHERE user_id = ? GROUP BY symbol;", session["user_id"])

    # Query database of user's balance
    user_balance = (db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"]))[0]["cash"]
    
    # For adding price of each stock to the user's balance
    total_cash = user_balance

    # Store other portfolio values
    for stock in range(len(portfolio)):
        # Get the current price of the stock
        get_qoute = lookup(portfolio[stock]["symbol"])

        # Store needed values for the portfolio
        portfolio[stock]["name"] = get_qoute["name"]
        portfolio[stock]["price"] = get_qoute["price"]
        portfolio[stock]["total"] = get_qoute["price"] * portfolio[stock]["SUM(shares)"]

        # Get total cash including prices of each stocks and user's balance
        total_cash = total_cash + portfolio[stock]["total"]

    # Remove stocks that have empty shares
    for stock in range(len(portfolio)):
        if portfolio[stock]["SUM(shares)"] == 0:
            del portfolio[stock]

    # Return list of stocks
    return render_template("index.html", portfolio=portfolio, user_balance=user_balance, total_cash=total_cash)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via Post
    if request.method == "POST":
        
        # Ensure symbol is valid
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("invalid input", 400)

        get_qoute = lookup(symbol)
        if not get_qoute:
            return apology("invalid symbol", 400)

        # Ensure shares is valid
        shares = request.form.get("shares")
        if not shares:
            return apology("invalid input", 400)

        if not shares.isdigit():
            return apology("invalid input", 400)

        # Query database for user's balance
        user_balance = (db.execute("SELECT cash FROM users WHERE id = ?;", session["user_id"]))[0]["cash"]

        # Total amount of transaction
        price = get_qoute["price"] * int(shares)
        new_balance = user_balance - price

        # Ensure user can buy it
        if new_balance < 0:
            return apology("invalid shares", 400)
        
        # Query database for new updated user's cash
        db.execute("UPDATE users SET cash = ? WHERE id = ?;", new_balance, session["user_id"])

        # Query database for new user's transaction
        db.execute("INSERT INTO transactions (user_id, symbol, shares, price) VALUES (?, ?, ?, ?);",
                   session["user_id"], get_qoute["symbol"], shares, price)

        # Redirect to home page
        return redirect("/")

    # User reached route via Post
    return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Query database for all transactions of the user
    history = db.execute("SELECT * FROM transactions WHERE user_id = ? ORDER BY date DESC", session["user_id"])

    # Return list of transactions
    return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    
    # User reached rout via POST
    if request.method == "POST":

        # Ensure input is valid
        get_quote = lookup(request.form.get("symbol"))
        if not get_quote:
            return apology("invalid symbol", 400)

        # Redirect to quoted stock
        return render_template("quoted.html", get_quote=get_quote)

    # User reached route via GET
    return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        username = request.form.get("username")
        if not username:
            return apology("must provide username", 400)

        # Ensure username doesn't exist
        elif len(db.execute("SELECT * FROM users WHERE username = ?", username)) == 1:
            return apology("username is already taken", 400)

        # Ensure password was submitted
        password = request.form.get("password")
        if not password:
            return apology("must provide password", 400)

        # Ensure password confirmation was submitted
        confirmation = request.form.get("confirmation")
        if not confirmation:
            return apology("must provide password confirmation", 400)

        # Ensure password and confirmation is matched
        if password != confirmation:
            return apology("password doesn't matched", 400)

        # Query database for new account
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, generate_password_hash(password))
            
        # Query database for username
        user = db.execute("SELECT * FROM users WHERE username = ?", username)

        # Remember which user has logged in
        session["user_id"] = user[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # Query database for all user's current stock symbols
    user_symbols = db.execute("SELECT symbol FROM transactions WHERE user_id = ? GROUP BY symbol;", session["user_id"])

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        
        # Ensure symbol is valid
        if not request.form.get("symbol"):
            return apology("invalid symbol", 400)

        get_qoute = lookup(request.form.get("symbol"))
        if not get_qoute:
            return apology("invalid symbol", 400)

        # Ensure user have the symbol
        found = 0
        for symbol in user_symbols:
            if symbol["symbol"] == get_qoute["symbol"]:
                found = 1

        if not found:
            return apology("symbol not found", 400)

        # Ensure shares is valid
        shares = request.form.get("shares")
        if not shares:
            return apology("invalid input", 400)

        if not shares.isdigit():
            return apology("invalid input", 400)

        # Ensure use have enough shares
        user_shares = db.execute(
            "SELECT SUM(shares) FROM transactions WHERE user_id = ? AND symbol = ? GROUP BY symbol", session["user_id"], get_qoute["symbol"])
        if int(shares) > user_shares[0]["SUM(shares)"]:
            return apology("not enough shares", 400)

        # Query database for user balance
        user_balance = db.execute("SELECT cash FROM users WHERE id = ?;", session["user_id"])

        # Total amount of transaction
        price = int(shares) * get_qoute["price"]
        new_balance = user_balance[0]["cash"] + price

        # Query database for new updated user's cash
        db.execute("UPDATE users SET cash = ? WHERE id = ?;", new_balance, session["user_id"])

        # Query database for new user's transaction
        db.execute("INSERT INTO transactions (user_id, symbol, shares, price) VALUES (?, ?, ?, ?);",
                   session["user_id"], get_qoute["symbol"], (int(shares) * -1), price)

        # Redirect to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    return render_template("sell.html", user_symbols=user_symbols)

