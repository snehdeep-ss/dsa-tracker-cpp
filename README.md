# DSA Tracker

A self-hosted DSA preparation platform with a C++ HTTP backend written from scratch.

## Architecture

- **HTTP Server**: Hand-rolled epoll event loop + thread pool (no frameworks)
- **Storage**: SQLite with Repository pattern abstraction
- **Spaced Repetition**: SM-2 algorithm
- **AI Integration**: Claude API for personalized study plan generation
- **Integrations**: LeetCode submission auto-fetch + GitHub auto-commit

## Setup

```bash
# 1. Clone and enter
git clone https://github.com/yourname/dsa-tracker.git
cd dsa-tracker

# 2. Configure
cp config/.env.example config/.env
# Edit config/.env with your API keys

# 3. Init database
sqlite3 dsa_tracker.db < db/schema.sql
sqlite3 dsa_tracker.db < scripts/seed.sql

# 4. Build
cmake -B build && cmake --build build -j$(nproc)

# 5. Run
./build/dsa_tracker
```

## Benchmarks
> TODO: add wrk benchmark results

## API Docs
See [API Reference](docs/api.md)
