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
COPY config.json .

# Create build directory
RUN mkdir build && cd build

# Build the application
RUN cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    make -j $(nproc)

# Create runtime image
FROM ubuntu:22.04

# Install runtime dependencies
RUN apt-get update && \
    apt-get install -y \
    gcc \
    g++ \
    git \
    cmake \
    libsqlite3-dev \
    libjsoncpp-dev \
    openssl \
    libssl3 \
    uuid-dev \
    postgresql-all \
    zlib1g-dev && \
    rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy the built executable and required files from builder
COPY --from=builder /app/build/your_project .
COPY --from=builder /app/static ./static
COPY --from=builder /app/config.json .

# Expose the port your app runs on
EXPOSE 5555

# Command to run the application
CMD ["./webcpp"]
