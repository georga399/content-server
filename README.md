# content_service
Server for images and files uploads.
## Requests
```sh
curl -X POST https://example.io/v1/upload/ -F "image=@/path/to/file" -H 'Authorization: Token {some_token}'
```
```sh
curl -X GET https://example.io/uploads/{filename}
```
```sh
curl -X DELETE https://example.io/v1/upload/{filename} -H 'Authorization: Token {some_token}'
```

C++ service that uses [userver framework](https://github.com/userver-framework/userver).

## License

The original template is distributed under the [Apache-2.0 License](https://github.com/userver-framework/userver/blob/develop/LICENSE)
and [CLA](https://github.com/userver-framework/userver/blob/develop/CONTRIBUTING.md). Services based on the template may change
the license and CLA.
