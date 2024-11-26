#!/usr/bin/env python3

import sqlite3
import time
import os

database_name = "transactions.db"


def create_database():
    if os.path.exists(database_name):
        os.remove(database_name)
        print("An old database removed.")
    connection = sqlite3.connect(database_name)
    cursor = connection.cursor()
    cursor.execute(""" CREATE TABLE transactions_log (
        log_time text,
        uid text,
        num text
    )""")
    connection.commit()
    connection.close()
    print("The new database created.")


if __name__ == "__main__":
    create_database()
