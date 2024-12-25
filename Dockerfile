FROM drogonframework/drogon 

WORKDIR /app

COPY . .

# Install any additional dependencies
# None for now

RUN 

RUN mkdir build && cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    cmake --build . 


CMD ["./webcpp"]

EXPOSE 5555
