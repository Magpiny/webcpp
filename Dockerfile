FROM drogonframework/drogon 

WORKDIR /app

COPY . .

# Install any additional dependencies
# None for now

RUN mkdir build && cd build
RUN cmake -DCMAKE_BUILD_TYPE=Release ..
RUN cmake --build .


CMD ["./build/webcpp"]

EXPOSE 5555
