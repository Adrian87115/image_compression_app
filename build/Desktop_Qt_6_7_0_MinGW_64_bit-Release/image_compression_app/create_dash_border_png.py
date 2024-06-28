from PIL import Image

# Create an image with 10x10 pixels
img = Image.new('RGBA', (10, 10), (255, 255, 255, 0))  # Transparent background

# Draw black and transparent pixels to form a dashed line pattern
pixels = img.load()
for i in range(10):
    if i % 2 == 0:
        for j in range(10):
            pixels[i, j] = (0, 0, 0, 255)  # Black color
    else:
        for j in range(10):
            pixels[i, j] = (255, 255, 255, 0)  # Transparent color

# Save the image
img.save('dashed_border.png')
