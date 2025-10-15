import sqlite3

def find_user_by_username(username):
    conn = sqlite3.connect("app.db")
    cursor = conn.cursor()
    
    query = "SELECT * FROM users WHERE username = ?"
    
    try:
        cursor.execute(query, (username,))
        user_record = cursor.fetchone()
        return user_record
    finally:
        conn.close()