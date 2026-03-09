-- DSA Tracker — SQLite Schema
-- Run: sqlite3 dsa_tracker.db < db/schema.sql

CREATE TABLE IF NOT EXISTS users (
    id              INTEGER PRIMARY KEY AUTOINCREMENT,
    name            TEXT NOT NULL,
    email           TEXT UNIQUE NOT NULL,
    password_hash   TEXT NOT NULL,
    onboarding_done INTEGER DEFAULT 0,
    created_at      DATETIME DEFAULT CURRENT_TIMESTAMP,
    last_active_at  DATETIME DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS problems (
    id                  INTEGER PRIMARY KEY AUTOINCREMENT,
    slug                TEXT UNIQUE NOT NULL,
    title               TEXT NOT NULL,
    url                 TEXT,
    difficulty          TEXT CHECK(difficulty IN ('easy','medium','hard')),
    source              TEXT CHECK(source IN ('leetcode','codeforces','custom')),
    description         TEXT,
    created_by_user_id  INTEGER REFERENCES users(id),
    created_at          DATETIME DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS tags (
    id   INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT UNIQUE NOT NULL
);

CREATE TABLE IF NOT EXISTS problem_tags (
    problem_id INTEGER REFERENCES problems(id),
    tag_id     INTEGER REFERENCES tags(id),
    PRIMARY KEY (problem_id, tag_id)
);

CREATE TABLE IF NOT EXISTS sheets (
    id              INTEGER PRIMARY KEY AUTOINCREMENT,
    slug            TEXT UNIQUE NOT NULL,
    name            TEXT NOT NULL,
    author          TEXT,
    description     TEXT,
    total_problems  INTEGER DEFAULT 0,
    created_at      DATETIME DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS sheet_problems (
    id          INTEGER PRIMARY KEY AUTOINCREMENT,
    sheet_id    INTEGER REFERENCES sheets(id),
    problem_id  INTEGER REFERENCES problems(id),
    section     TEXT,
    order_index INTEGER,
    notes       TEXT
);

CREATE TABLE IF NOT EXISTS user_sheet_subscriptions (
    id            INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id       INTEGER REFERENCES users(id),
    sheet_id      INTEGER REFERENCES sheets(id),
    status        TEXT DEFAULT 'active' CHECK(status IN ('active','paused','completed')),
    subscribed_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    completed_at  DATETIME,
    UNIQUE(user_id, sheet_id)
);

CREATE TABLE IF NOT EXISTS user_sheet_progress (
    id              INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id         INTEGER REFERENCES users(id),
    problem_id      INTEGER REFERENCES problems(id),
    sheet_id        INTEGER REFERENCES sheets(id),
    status          TEXT DEFAULT 'unsolved' CHECK(status IN ('unsolved','attempted','solved')),
    solve_count     INTEGER DEFAULT 0,
    last_solved_at  DATETIME,
    notes           TEXT,
    ease_factor     REAL DEFAULT 2.5,
    interval_days   INTEGER DEFAULT 1,
    next_review_at  DATE,
    created_at      DATETIME DEFAULT CURRENT_TIMESTAMP,
    UNIQUE(user_id, problem_id, sheet_id)
);

CREATE INDEX IF NOT EXISTS idx_progress_review  ON user_sheet_progress(user_id, next_review_at);
CREATE INDEX IF NOT EXISTS idx_progress_status  ON user_sheet_progress(user_id, status);

CREATE TABLE IF NOT EXISTS user_plans (
    id                  INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id             INTEGER REFERENCES users(id),
    status              TEXT DEFAULT 'active' CHECK(status IN ('active','completed','abandoned')),
    target              TEXT,
    daily_time_minutes  INTEGER,
    level               TEXT,
    weak_areas          TEXT,
    timeline_days       INTEGER,
    total_problems      INTEGER,
    ai_raw_response     TEXT,
    created_at          DATETIME DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS user_plan_items (
    id          INTEGER PRIMARY KEY AUTOINCREMENT,
    plan_id     INTEGER REFERENCES user_plans(id),
    problem_id  INTEGER REFERENCES problems(id),
    day_number  INTEGER,
    reason      TEXT,
    status      TEXT DEFAULT 'pending' CHECK(status IN ('pending','solved','skipped'))
);

CREATE TABLE IF NOT EXISTS submissions (
    id                  INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id             INTEGER REFERENCES users(id),
    problem_id          INTEGER REFERENCES problems(id),
    leetcode_sub_id     TEXT,
    language            TEXT,
    code                TEXT,
    runtime_ms          INTEGER,
    memory_kb           INTEGER,
    lc_submitted_at     DATETIME,
    github_commit_url   TEXT,
    github_commit_sha   TEXT,
    fetch_status        TEXT DEFAULT 'pending' CHECK(fetch_status IN ('pending','success','failed')),
    commit_status       TEXT DEFAULT 'pending' CHECK(commit_status IN ('pending','success','failed','skipped')),
    created_at          DATETIME DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS user_integrations (
    id               INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id          INTEGER REFERENCES users(id),
    service          TEXT CHECK(service IN ('leetcode','github')),
    username         TEXT,
    encrypted_token  TEXT,
    github_repo_url  TEXT,
    is_active        INTEGER DEFAULT 1,
    connected_at     DATETIME DEFAULT CURRENT_TIMESTAMP,
    last_used_at     DATETIME,
    UNIQUE(user_id, service)
);
