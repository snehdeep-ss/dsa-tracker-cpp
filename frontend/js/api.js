// api.js — thin fetch() wrapper around the C++ backend
const BASE = 'http://localhost:8080/api';
const token = () => localStorage.getItem('token');

const api = {
  get:    (path)       => fetch(`${BASE}${path}`, { headers: { Authorization: `Bearer ${token()}` } }).then(r => r.json()),
  post:   (path, body) => fetch(`${BASE}${path}`, { method: 'POST',  headers: { 'Content-Type': 'application/json', Authorization: `Bearer ${token()}` }, body: JSON.stringify(body) }).then(r => r.json()),
  patch:  (path, body) => fetch(`${BASE}${path}`, { method: 'PATCH', headers: { 'Content-Type': 'application/json', Authorization: `Bearer ${token()}` }, body: JSON.stringify(body) }).then(r => r.json()),
  delete: (path)       => fetch(`${BASE}${path}`, { method: 'DELETE', headers: { Authorization: `Bearer ${token()}` } }).then(r => r.json()),
};
