# Kiisu Combined Firmware — Project Notes

## What This Is
Combined Flipper Zero firmware: Momentum + Kiisu v4b hardware + 600+ apps from RogueMaster.
Branch: `kiisu-dev` on `apmlabs/Momentum-Firmware`

## CRITICAL RULES
1. **NEVER** run `git submodule update` on `applications/external` — it's tracked as regular files
2. **ALWAYS** use `FBT_NO_SYNC=1` when building (skips submodule sync)
3. External apps are sandboxed `.fap` files — they cannot brick the device

## Build
```bash
FBT_NO_SYNC=1 ./fbt updater_package        # Full 17MB .tgz
FBT_NO_SYNC=1 ./fbt fap_<appid>            # Single app
FBT_NO_SYNC=1 ./fbt updater_package --keep-going  # Show all errors
```

## API: Momentum vs RogueMaster
When adding apps from RogueMaster, these changes are needed:
- `cfw/cfw.h` → `momentum/momentum.h`, `cfw_settings` → `momentum_settings`
- `Cli*` → `CliRegistry*`, `cli_add_command` → `cli_registry_add_command`
- `elements_scrollable_text_line`: 8 args → 7 (remove last bool)
- `const` on SPI/I2C handles — cast with `(FuriHalSpiBusHandle*)`
- `CdcCallbacks`: 6 fields → 5 (no `break_callback`)
- `power_reboot(mode)` → `power_reboot(Power*, mode)`
- `getRandomDeed()` → `DolphinDeedPluginGameWin`
- `nrf24_HANDLE` macro: must be multi-line with proper `#ifdef`/`#else`/`#endif`

## Apps Removed (incompatible)
f0forth, flipper95, upython, cli_bridge, extra_demo, can_fd — all need internal CLI APIs

## Releases
- kiisu-mntm-013-apps: 661 apps, 17MB (the good one)
- kiisu-mntm-012-fix1: Momentum + Kiisu only, Tamagotchi fix
- kiisu-mntm-012: Base Momentum + Kiisu merge
