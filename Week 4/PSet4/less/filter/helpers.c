#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Go through the each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int red = image[i][j].rgbtRed;
            int green = image[i][j].rgbtGreen;
            int blue = image[i][j].rgbtBlue;
            
            // Find the average
            int gray = round((red + blue + green) / 3.0);
            
            // Set all value as the Average
            image[i][j].rgbtRed = image[i][j].rgbtGreen = image[i][j].rgbtBlue = gray;
        }
    }
    
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // Go through the each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int red = image[i][j].rgbtRed;
            int green = image[i][j].rgbtGreen;
            int blue = image[i][j].rgbtBlue;
            
            // Set the Sepia value
            int sepiaRed = round(0.393 * red + 0.769 * green + 0.189 * blue);
            if (sepiaRed > 255)
            {
                image[i][j].rgbtRed = 255;
            }
            else
            {
                image[i][j].rgbtRed = sepiaRed;
            }
            
            int sepiaGreen = round(0.349 * red + 0.686 * green + 0.168 * blue);
            if (sepiaGreen > 255)
            {
                image[i][j].rgbtGreen = 255;
            }
            else
            {
                image[i][j].rgbtGreen = sepiaGreen;
            }
            
            int sepiaBlue = round(0.272 * red + 0.534 * green + 0.131 * blue);
            if (sepiaBlue > 255)
            {
                image[i][j].rgbtBlue = 255;
            }
            else
            {
                image[i][j].rgbtBlue = sepiaBlue;
            }
        }
    }
    
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Go through the each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < (width / 2); j++)
        {
            // Interchange pixel to the other side
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - (j + 1)];
            image[i][width - (j + 1)] = temp;
        }
    }
    
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE blur[height][width];
    
    // Go through the each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sumRed = 0;
            int sumGreen = 0;
            int sumBlue = 0;
            int neighbors = 0; 
            
            // Get the values around it
            for (int x = -1; x < 2; x++)
            {
                for (int y = -1; y < 2; y++)
                {
                    int currentX = i + x;
                    int currentY = j + y;
                    
                    // Skip if outside the image
                    if (currentX < 0 || currentX > (height - 1) || currentY < 0 || currentY > (width - 1))
                    {
                        continue;
                    }
                    
                    // Get the value
                    sumRed += image[currentX][currentY].rgbtRed;
                    sumGreen += image[currentX][currentY].rgbtGreen;
                    sumBlue += image[currentX][currentY].rgbtBlue;
                    
                    neighbors++;
                }
            }
                
            // Get the average that serve as the blue
            blur[i][j].rgbtRed = round((float) sumRed / neighbors);
            blur[i][j].rgbtGreen = round((float) sumGreen / neighbors);
            blur[i][j].rgbtBlue = round((float) sumBlue / neighbors);
        }
    }
    
    // Replace the actual image to blur
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = blur[i][j].rgbtRed;
            image[i][j].rgbtGreen = blur[i][j].rgbtGreen;
            image[i][j].rgbtBlue = blur[i][j].rgbtBlue;
        }
    }
    return;
}
