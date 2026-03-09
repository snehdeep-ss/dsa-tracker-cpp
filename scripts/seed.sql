-- Seed curated sheets
-- Run after schema.sql: sqlite3 dsa_tracker.db < scripts/seed.sql

INSERT OR IGNORE INTO sheets (slug, name, author, description, total_problems) VALUES
  ('neetcode-150', 'NeetCode 150', 'NeetCode',  'Curated 150 problems covering all major patterns', 150),
  ('striver-sde',  'Striver SDE Sheet', 'Striver', '191 problems for SDE interviews at top companies', 191),
  ('blind-75',     'Blind 75', 'community', 'The classic 75 problems every engineer should know', 75);

-- Seed common tags
INSERT OR IGNORE INTO tags (name) VALUES
  ('arrays'), ('strings'), ('hash-map'), ('two-pointers'), ('sliding-window'),
  ('binary-search'), ('linked-list'), ('trees'), ('graphs'), ('dp'),
  ('backtracking'), ('greedy'), ('heap'), ('trie'), ('union-find'),
  ('recursion'), ('divide-and-conquer'), ('bit-manipulation'), ('math'), ('stack');
