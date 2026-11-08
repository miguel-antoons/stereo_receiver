# stereo_receiver

A stereo receiver built to run on an ESP32 (Arduino framework, built with [PlatformIO]):
Bluetooth (A2DP) audio input, software volume, and I2S output to an external DAC/amp
(PCM5102A / MA12070P).

## Project layout

```
firmware/          PlatformIO project (Arduino framework on ESP32)
  src/             application code
  platformio.ini   build environments: esp32_wroom (default), esp32_wrover
  .clang-format    clang-format style
  prek.toml        prek git hook config (clang-format + pio check)
```

## Developer setup

### 1. What to install (required to compile)

- **PlatformIO Core (CLI)** — drives the build and downloads the ESP32 toolchain
  (xtensa-esp-elf GCC + Arduino core) automatically on the first build:

  ```sh
  # official installer (recommended); afterwards add ~/.platformio/bin to your PATH
  curl -fsSL -o get-platformio.py \
    https://raw.githubusercontent.com/platformio/platformio-core-installer/main/python/get-platformio.py
  python3 get-platformio.py

  # or: pipx install platformio   (then ~/.local/bin must be on your PATH)
  ```

  VS Code users can alternatively install the *PlatformIO IDE* extension, which
  bundles PlatformIO Core and manages the toolchain the same way.

- **clangd** — only needed for editor code intelligence, not for compiling.
  Zed downloads and manages clangd by itself. For other editors install the
  `clangd` package: Debian/Ubuntu `sudo apt install clangd`,
  Fedora `sudo dnf install clang-tools-extra`, macOS `brew install llvm`.

### 2. Build

```sh
cd firmware
pio run                    # build the default env (esp32_wroom)
pio run -e esp32_wrover    # build the WROVER variant
pio run -t upload          # flash the board
pio device monitor         # serial monitor (115200 baud)
```

The first build installs the toolchain and generates `firmware/compile_commands.json`,
which is what clangd reads. Re-run `pio run` whenever `platformio.ini` changes.

### 3. Editor setup (clangd)

`compile_commands.json`, `firmware/.clangd` and `.zed/` are **gitignored** because
they contain machine-specific absolute paths — every developer creates them locally
as described below. To understand the ESP32 cross-compilation, clangd needs:

1. `--query-driver` pointing at the PlatformIO xtensa toolchain, so it is allowed to
   ask the cross-compiler for its built-in include paths, and
2. the toolchain's GCC headers added as `-isystem` (the `firmware/.clangd` file).

The toolchain path depends on your PlatformIO installation and GCC version. Check
yours with:

```sh
ls ~/.platformio/packages/toolchain-xtensa-esp-elf/lib/gcc/xtensa-esp-elf/
```

(On Windows PlatformIO lives in `%USERPROFILE%\.platformio` instead of `~/.platformio`.)

#### Zed

Create `.zed/settings.json` in the repository root:

```json
{
  "lsp": {
    "clangd": {
      "binary": {
        "arguments": [
          "--query-driver=/home/<you>/.platformio/packages/toolchain-xtensa-esp-elf/bin/*"
        ]
      }
    }
  }
}
```

Use the absolute path to your home directory — `~` is not expanded.

#### VS Code

Install the **clangd** extension (`llvm-vs-code-extensions.vscode-clangd`). If you
open the repository root (not just `firmware/`), point clangd at the build directory
too, in `.vscode/settings.json`:

```json
{
  "clangd.arguments": [
    "--compile-commands-dir=${workspaceFolder}/firmware",
    "--query-driver=/home/<you>/.platformio/packages/toolchain-xtensa-esp-elf/bin/*"
  ]
}
```

Opening `firmware/` as the workspace folder also works and then only needs the
`--query-driver` argument.

#### Neovim / any other clangd-based editor

Add the same flag to your clangd LSP arguments, e.g. with nvim-lspconfig:

```lua
require("lspconfig").clangd.setup({
  cmd = {
    "clangd",
    "--query-driver=" .. os.getenv("HOME") .. "/.platformio/packages/toolchain-xtensa-esp-elf/bin/*",
  },
})
```

clangd finds `firmware/compile_commands.json` automatically by walking up from the
source files, so any editor with clangd support works with just this flag.

#### `firmware/.clangd`

Create `firmware/.clangd` so clangd finds the cross-compiler's built-in headers:

```yaml
CompileFlags:
  Add:
    - -isystem
    - /home/<you>/.platformio/packages/toolchain-xtensa-esp-elf/lib/gcc/xtensa-esp-elf/<gcc-version>/include
```

Replace `<gcc-version>` with the directory listed by the `ls` command above
(e.g. `14.2.0`).

### 4. Linting & static analysis

Three tools are configured in `firmware/`:

- **clang-format** (`firmware/.clang-format`) — code formatting (Google-based
  style, 4-space indentation).
- **cppcheck + clang-tidy** (`check_tool` in `firmware/platformio.ini`) — static
  analysis, driven by `pio check`.
- **prek** (`firmware/prek.toml`) — a git hook framework ([prek], a Rust
  rewrite of pre-commit) that runs both of the above on every commit.

#### What to install (only needed for linting)

PlatformIO drives cppcheck and clang-tidy but does not install them, and the
prek hooks call all tools directly from your `PATH`:

```sh
# Debian/Ubuntu
sudo apt install clang-format clang-tidy cppcheck

# Fedora
sudo dnf install clang-tools-extra cppcheck

# macOS
brew install clang-format clang-tidy cppcheck
```

Plus **prek** itself if you want the git hooks:

```sh
brew install prek    # or: pipx install prek, or: cargo install prek --locked
```

#### Running the linters

```sh
cd firmware
pio check --fail-on-defect=medium   # cppcheck + clang-tidy (what the hook runs)
prek run --all-files                # both hooks: clang-format + pio check
prek install                        # install the git hooks (once per clone)
```

- `pio check` lints the project sources (`check_skip_packages = yes` skips
  library dependencies) and fails on medium-severity defects or higher.
- `prek run` without `--all-files` checks only the staged files.
- `prek install` runs both hooks automatically on every commit; the
  clang-format hook rewrites files in place (`clang-format -i`), so a failed
  commit usually just needs the reformatted files to be re-staged.

[PlatformIO]: https://platformio.org
[prek]: https://prek.j178.dev
