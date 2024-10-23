import http.server
import socketserver


# 定义请求处理类
class MyHandler(http.server.BaseHTTPRequestHandler):
    def do_POST(self):
        # 获取请求的长度
        content_length = int(self.headers['Content-Length'])
        # 读取并打印收到的数据
        post_data = self.rfile.read(content_length)
        print(f"Received POST data: {post_data.decode('utf-8')}")

        # 返回HTTP 200响应
        self.send_response(200)
        self.end_headers()
        self.wfile.write(b"Data received")

    def do_GET(self):
        # 处理GET请求
        self.send_response(200)
        self.end_headers()
        self.wfile.write(b"Send data via POST to this server")


# 启动服务器
HOST = "127.0.0.1"
PORT = 7899

with socketserver.TCPServer((HOST, PORT), MyHandler) as httpd:
    print(f"Serving on {HOST}:{PORT}")
    httpd.serve_forever()
