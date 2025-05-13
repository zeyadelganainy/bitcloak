from PIL import Image

def create_png(width, height, filename):
    img = Image.new('RGB', (width, height), color='white')
    img.save(filename)

# Generate PNGs of various sizes
sizes = [
    (640, 480),    # Small
    (1280, 720),   # Medium
    (1920, 1080),  # Large
    (3840, 2160)   # Extra Large (4K)
]

for width, height in sizes:
    filename = f"test_{width}x{height}.png"
    create_png(width, height, filename)
    print(f"Created {filename}")