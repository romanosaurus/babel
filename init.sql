CREATE TABLE IF NOT EXISTS users(id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, username VARCHAR(255) UNIQUE, password VARCHAR(255));
CREATE TABLE IF NOT EXISTS friends(first_id INTEGER, second_id INTEGER, FOREIGN KEY(first_id) REFERENCES users(id), FOREIGN KEY(second_id) REFERENCES users(id));

ALTER TABLE users ADD email TEXT;
ALTER TABLE users ADD date_born BIGINT;

CREATE TABLE IF NOT EXISTS friends_requests(from_id INTEGER, to_id INTEGER, FOREIGN KEY(from_id) REFERENCES users(id), FOREIGN KEY(to_id) REFERENCES users(id));

CREATE TABLE IF NOT EXISTS messages(from_id INTEGER, to_id INTEGER, time_created BIGINT, message TEXT, FOREIGN KEY(from_id) REFERENCES users(id), FOREIGN KEY(to_id) REFERENCES users(id))