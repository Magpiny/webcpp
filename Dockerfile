FROM drogonframework/drogon:latest as builder

# Set working directory
WORKDIR /app

# Copy CMakeLists.txt and source code
COPY CMakeLists.txt .
COPY main.cc .
COPY controllers/ ./controllers/
COPY views/ ./views/
COPY static/ ./static/
COPY models/ ./models/
COPY plugins/ ./plugins/
COPY filters/ ./filters/
COPY test/ ./test/
COPY config.json .

# Build the application
RUN mkdir build && \
    cd build && \
    cmake .. && \
    cmake --build .

# Create runtime image
FROM ubuntu:22.04

# Install runtime dependencies
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y \
    gcc \
    g++ \
    git \
    cmake \
    libsqlite3-dev \
    libjsoncpp-dev \
    openssl \
    libssl3 \
    uuid-dev \
    libc-ares2 \
    libc-ares-dev \
    libmariadb3 \
    libmariadb-dev-compat \
    libmariadb-dev \
    libhiredis0.14 \
    libhiredis-dev \
    postgresql-all \
    zlib1g-dev && \
    rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy the built executable and required files from builder
COPY --from=builder /app/build/webcpp .
COPY --from=builder /app/static ./static
COPY --from=builder /app/views ./views
COPY --from=builder /app/config.json .

# Expose the port your app runs on
EXPOSE 5555

# Command to run the application
CMD ["./webcpp"]
