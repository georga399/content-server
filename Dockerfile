FROM ghcr.io/userver-framework/ubuntu-22.04-userver-pg:latest

WORKDIR /content-service

COPY . .

RUN apt-get install -y nginx

COPY ./nginx/nginx.conf /etc/nginx/nginx.conf
RUN chmod +x ./deploy/start.sh
RUN mkdir /uploads && chown -R www-data:www-data /uploads

EXPOSE 80 8080

RUN cmake -B build_release -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
RUN cmake --build build_release -j $(nproc) --target content_service
ENTRYPOINT ["./deploy/start.sh" ] 

