CREATE TABLE clients (
    client_id INTEGER PRIMARY KEY,
    client_name TEXT NOT NULL UNIQUE,
    device_type TEXT,
	client_ip TEXT,
    registration_timestamp TEXT DEFAULT CURRENT_TIMESTAMP,
    last_seen TEXT,
    metadata TEXT
);

CREATE TABLE device_types (
    device_type TEXT PRIMARY KEY,
    description TEXT
);

CREATE TABLE sessions (
    session_id INTEGER PRIMARY KEY,
    client_id TEXT NOT NULL,
    start_time TEXT NOT NULL,
    end_time TEXT,
    ip_address TEXT,
    session_status TEXT DEFAULT 'active',
    FOREIGN KEY (client_id) REFERENCES clients(client_id)
);

CREATE TABLE session_logs (
    log_id INTEGER PRIMARY KEY,
    session_id TEXT NOT NULL,
    timestamp TEXT DEFAULT CURRENT_TIMESTAMP,
    log_level TEXT,      
    message TEXT,
    metadata TEXT,        -- additional JSON details
    FOREIGN KEY (session_id) REFERENCES sessions(session_id)
);