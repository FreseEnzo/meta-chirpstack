#!/bin/sh

# Exit immediately if any command fails
set -e

echo "Starting PostgreSQL initialization..."

# Initialize PostgreSQL database
echo "Initializing PostgreSQL database..."
sudo -u postgres initdb -D /var/lib/postgresql/data
echo "Database initialization complete."

# Start PostgreSQL service
echo "Starting PostgreSQL service..."
systemctl start postgresql
echo "PostgreSQL service started."

# Wait for PostgreSQL to start
echo "Checking PostgreSQL connection..."
while ! pg_isready -q -h localhost -p 5432; do
    echo "Waiting for PostgreSQL to start..."
    sleep 2
done
echo "PostgreSQL is now ready."

# Create ChirpStack role and database
echo "Creating ChirpStack role and database..."
sudo -u postgres psql << EOF
CREATE ROLE chirpstack WITH LOGIN PASSWORD 'chirpstack';
CREATE DATABASE chirpstack WITH OWNER chirpstack;
\c chirpstack
CREATE EXTENSION pg_trgm;
EOF
echo "ChirpStack database setup complete."

echo "PostgreSQL initialization and setup finished successfully."