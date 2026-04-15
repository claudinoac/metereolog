
# Core Settings
bind = "0.0.0.0:8009"
workers = 4
worker_class = "sync"

# Logging & Process Info
accesslog = "-"
errorlog = "-"
loglevel = "info"
proc_name = "metereolog"

# Performance
max_requests = 1000 #
timeout = 30
