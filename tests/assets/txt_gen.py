import os

def create_file(size_in_kb, filename):
    with open(filename, 'wb') as f:
        f.write(os.urandom(size_in_kb * 1024))

# Generate files of various sizes
sizes = [1, 10, 100, 1024, 10240, 102400]  # Sizes in KB: 1KB, 10KB, 100KB, 1MB, 10MB, 100MB

for size in sizes:
    filename = f"file_{size}KB.txt"
    create_file(size, filename)
    print(f"Created {filename}")