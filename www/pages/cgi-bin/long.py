# !/usr/bin/env python3

import time
print("Content-Type: text/html\n")

print("<html><body style=\"text-align:center\"><h1>Long Test CGI</h1></body></html>")

while True:
	print("<p>Still running...</p>")
	time.sleep(5)