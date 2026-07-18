# pmgr

A lightweight CLI process manager for systemd services. Define groups of units in a config file and manage them together with a single command.

## Installation

### From release (.deb)

```bash
wget https://github.com/adqm0001/pmgr/releases/latest/download/pmgr_0.1.0.deb
sudo dpkg -i pmgr_0.1.0.deb
```

### From source

```bash
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

| Command | Description |
| --- | --- |
| `pmgr help` | Show the list of available commands |
| `pmgr init <group> [units]` | Create the config file and add a group with optional units |
| `pmgr status <group>` | Show `systemctl status` for every unit in the group |
| `pmgr logs <group>` | Show `journalctl` logs for every unit in the group |
| `pmgr logs <group> -n <number>` | Show the last `<number>` log lines per unit |
| `pmgr restart <group>` | Restart every unit in the group |
| `pmgr custom <group>` | List the custom commands defined for the group |
| `pmgr run <group> <command>` | Run one of the group's custom commands |
| `pmgr create-custom <group> <command> "<shell command>" -d "<description>"` | Add a custom command to the group (description optional) |
| `pmgr delete-custom <group> <command>` | Remove a custom command from the group |
| `pmgr delete-group <group>` | Remove the group and its custom commands from the config |
| `pmgr view-config` | Print the contents of the config file |
| `pmgr reset` | Delete the config file so you can start over with `init` |

## Notes

- Most commands require `sudo`
- Units are comma-separated in the config
- Units are optional at init; add them later by editing the config
- Custom command descriptions are optional
