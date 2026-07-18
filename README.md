# pmgr

A lightweight CLI process manager for systemd services. Define groups of units in a config file and manage them together with a single command.

## Installation

```bash
make
make install
```

## Config file

Located at `/etc/pmgr/config.ini`. Create it manually or use `pmgr init`.

```ini
[cu-scraper]
units = cu-scraper-api, cu-scraper-poller

[cu-scraper:commands]
deploy = git pull && sudo systemctl restart cu-scraper-api | Deploy the app
```

## Usage

```
pmgr help
pmgr init <group> [units]
pmgr status <group>
pmgr logs <group>
pmgr logs <group> -n <number>
pmgr restart <group>
pmgr custom <group>
pmgr run <group> <command>
pmgr create-custom <group> <command> "<shell command>" -d "<description>"
pmgr delete-custom <group> <command>
pmgr delete-group <group>
```

## Notes

- Most commands require `sudo`
- Units are comma-separated in the config
- Units are optional at init; add them later by editing the config
- Custom command descriptions are optional
