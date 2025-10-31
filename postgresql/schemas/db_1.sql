DROP SCHEMA IF EXISTS url_schema CASCADE;

CREATE SCHEMA IF NOT EXISTS url_schema;

CREATE TABLE IF NOT EXISTS url_schema.urls (
    id SERIAL PRIMARY KEY,
    short_url TEXT UNIQUE NOT NULL,
    original_url TEXT NOT NULL,
    created_at TIMESTAMP DEFAULT now()
);
