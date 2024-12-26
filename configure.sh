#!/bin/bash

# Get the directory where configure.sh is run
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
LIBRARY_PATH="$SCRIPT_DIR/toralize.so"
BINARY_PATH="/usr/bin/toralize"
USAGE_MESSAGE="Usage: toralize <program>"

# Ensure the script is run as root
if [[ $EUID -ne 0 ]]; then
    echo "This script must be run as root. Use sudo ./configure.sh"
    exit 1
fi

# Build the shared library
echo "Running 'make build' to compile the shared library..."
make build
if [[ $? -ne 0 ]]; then
    echo "Error: 'make build' failed. Please check the Makefile and dependencies."
    exit 1
fi

# Create the bash script in /usr/bin/toralize
echo "Creating toralize script in $BINARY_PATH..."
cat << EOF > "$BINARY_PATH"
#!/bin/bash

export LD_PRELOAD=$LIBRARY_PATH

\${@}

unset LD_PRELOAD
EOF

# Set permissions to 755
chmod 755 "$BINARY_PATH"
echo "Set executable permissions (755) for $BINARY_PATH."

# Print the usage message
echo "Configuration completed successfully!"
echo "$USAGE_MESSAGE"
