# Spectrum Check

All-in-one Sub-GHz analyzer for Kiisu v4b / Flipper Zero. Combines spectrum visualization, frequency detection, signal decoding, and waveform viewing in a single app.

## Views

Switch between views with **Up/Down**.

### 1. Spectrum
Real-time RF sweep across the selected band. Bars show signal strength per channel, peak frequency highlighted with dBm value.

- **Left/Right**: Shift center frequency
- **OK**: Cycle width mode (Wide 20MHz / Narrow 4MHz / UltraWide 80MHz)

### 2. Freq Analyzer
Scans common Sub-GHz frequencies for active signals. Detected frequencies are logged with hit count and max RSSI.

- **Left/Right**: Adjust RSSI trigger threshold
- **OK**: Cycle sort mode (Count / RSSI / Freq / Recent)

### 3. Decoder
Captures raw pulses on the detected frequency and analyzes timing. Shows pulse count, duration, estimated symbol rate, and possible data rates.

- **OK**: Cycle modulation (OOK650 / OOK270 / 2FSK)

### 4. Waveform
Oscilloscope-style view of captured pulses. Scroll through the signal and zoom in to see individual pulse timing.

- **Left/Right**: Scroll through samples
- **OK**: Cycle zoom level (1x–4x)

## How It Works

The app uses the CC1101 radio in different modes depending on the active view:
- **Spectrum**: Rapid frequency hopping with RSSI measurement per channel
- **Freq Analyzer**: Scans 17 common frequencies, triggers on signals above threshold
- **Decoder/Waveform**: Async RX capture of raw pulse data on the locked frequency

All views share state — lock onto a signal in the Freq Analyzer, then switch to Decoder to analyze it, or Waveform to see the raw pulses.

## Install

The app is included in the firmware package. After flashing, find it under **Sub-GHz → Spectrum Check**.

Or copy `spectrum_check.fap` to your SD card at `apps/Sub-GHz/`.

## Building

```bash
cd /home/ubuntu/mcpprojects/kiisu-firmware
FBT_NO_SYNC=1 ./fbt fap_spectrum_check
# Output: build/f7-firmware-C/.extapps/spectrum_check.fap
```
