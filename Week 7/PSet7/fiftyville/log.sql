-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Theft took place on July 28, 2021
-- Theft took place on Humphrey Street

-- Suggestions
-- Explore table schemas to understand what data is available and how tables connect to one another
-- To query accross mutiple tables, nest queries together or join mutiple tables together.
-- Maintain a list of suspects

-- Get description of the crime
---------------------------------------------------------------------
SELECT description
FROM crime_scene_reports
WHERE year = 2021
AND month = 7
AND day = 28
AND street = "Humphrey Street";
---------------------------------------------------------------------

-- There 2 crime but only is connected to the crime
-- We got the time: 10:15am
-- We got the place: bakery in the same street

-- Now, I should get the transcript of all witnesses
---------------------------------------------------------------------
SELECT name, transcript 
FROM interviews
WHERE year = 2021
AND month = 7
AND day = 28;
---------------------------------------------------------------------

-- Filter all that is related to the thief
---------------------------------------------------------------------
SELECT name, transcript 
FROM interviews
WHERE year = 2021
AND month = 7
AND day = 28
AND transcript LIKE "%thief%";
-- Result: Ruth, Eugene, and Raymond
---------------------------------------------------------------------

-- Witness #1 Ruth, Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame. 
-- Witness #2 Eugene,I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money. 
-- Witness #3 Raymond, As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket. 

-- Clues:
-- Clue #1 = bakery, parking lot within 10 minutes
-- Clue #2 = Bank, withdrawing money in Leggett Street
-- Clue #3 = Calls, in < 1 minute
-- Clue #4 = Flight, tomorrow, eariest, Fiftyville, ACCOMPLICE purchased tickets

-- Found you Thief!
---------------------------------------------------------------------
SELECT name FROM people

-- Clue #1
WHERE license_plate IN (
  SELECT license_plate FROM bakery_security_logs
  WHERE year = 2021
  AND month = 7
  AND day = 28
  AND hour = 10
  AND minute > 15 AND minute < 25
)

-- Clue #2
AND id IN (
  SELECT person_id FROM bank_accounts
  WHERE account_number IN (
    SELECT account_number FROM atm_transactions
    WHERE year = 2021
    AND month = 7
    AND day = 28
    AND atm_location = "Leggett Street"
    AND transaction_type = "withdraw"
  )
)

-- Clue #3
AND phone_number in (
  -- Witness #3 statement "they called someone..." means they are the caller
  SELECT caller FROM phone_calls 
  WHERE year = 2021 
  AND month = 7 
  AND day = 28 
  -- I don't know if the duration is in minutes, but I'll assume that
  -- Edit: nothing shows up, so its probably in seconds
  AND duration < 60
)

-- Clue #4
AND passport_number IN (
  SELECT passport_number FROM passengers
  WHERE flight_id IN (
    SELECT id FROM flights
    WHERE year = 2021
    AND month = 7
    -- Witness #3 statement "earliest flight out of Fiftyville tomorrow..."
    AND day = 29
    ORDER BY hour, minute
    -- The thief being the first one
    LIMIT 1
  )
);
-- Result: Bruce
---------------------------------------------------------------------

-- Thief: Bruce

-- The city the thief escaped to
---------------------------------------------------------------------
-- Clue #4
SELECT city FROM airports
JOIN flights ON flights.destination_airport_id = airports.id
WHERE flights.origin_airport_id IN (
  SELECT id FROM airports
  WHERE city = "Fiftyville"
)
AND flights.year = 2021
AND flights.month = 7
-- The day they will leave
AND flights.day = 29
ORDER BY flights.hour, flights.minute
LIMIT 1;
-- Result: New York City
---------------------------------------------------------------------

-- Thief: Bruce
-- The city the thief excaped to: New York City 

-- Find Accomplice
---------------------------------------------------------------------
-- Clue #3
SELECT name from people
WHERE phone_number IN (
  SELECT receiver FROM phone_calls
  WHERE caller IN (
    SELECT phone_number FROM people
    WHERE name = "Bruce"
  )
  AND year = 2021
  AND month = 7
  AND day = 28
  AND duration < 60
);
-- Result: Robin
---------------------------------------------------------------------

-- SUMMARIZE RESULT
-- Thief: Bruce
-- The city the thief escaped to: New York City 
-- Accomplice: Robin

