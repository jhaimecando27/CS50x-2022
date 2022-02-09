#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Initialize Variables
            int red = image[i][j].rgbtRed;
            int green = image[i][j].rgbtGreen;
            int blue = image[i][j].rgbtBlue;
            
            // Compute for Grayscale
            int avg = round((red + green + blue) / 3.0);
            
            // Assign new values
            image[i][j].rgbtRed = image[i][j].rgbtGreen = image[i][j].rgbtBlue = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < (width / 2); j++)
        {
            // Store in temp
            RGBTRIPLE temp = image[i][width - (j + 1)];
            
            // Move 1st to 2nd
            image[i][width - (j + 1)] = image[i][j];
            
            // Move temp to 1st
            image[i][j] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE blur[height][width];
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Initialize Variables
            int sumRed = 0;
            int sumGreen = 0;
            int sumBlue = 0;
            int count = 0;
            
            // Iterate through neighbors
            for (int y = -1; y < 2; y++)
            {
                for (int x = -1; x < 2; x++)
                {
                    // 3x3 position
                    int currY = i + y;
                    int currX = j + x;
                    
                    // Skip if outside the image
                    if (currY < 0 || currX < 0 || currY > (height - 1) || currX > (width - 1))
                    {
                        continue;
                    }
                    
                    // Sum all neighbors
                    sumRed += image[currY][currX].rgbtRed;
                    sumGreen += image[currY][currX].rgbtGreen;
                    sumBlue += image[currY][currX].rgbtBlue;
                    
                    // Update number of neighbors
                    count++;
                }
            }
            
            
            // Compute & Assign new values
            blur[i][j].rgbtRed = round((float) sumRed / count);
            blur[i][j].rgbtGreen = round((float) sumGreen / count);
            blur[i][j].rgbtBlue = round((float) sumBlue / count);
        }
    }
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = blur[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE edge[height][width];
    
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int GxRed = 0;
            int GyRed = 0;
            int GxGreen = 0;
            int GyGreen = 0;
            int GxBlue = 0;
            int GyBlue = 0;
            
            for (int y = -1; y < 2; y++)
            {
                for (int x = -1; x < 2; x++)
                {
                    if (i + y < 0 || i + y > height - 1)
                }
            }
        }
    }
    return;
}
