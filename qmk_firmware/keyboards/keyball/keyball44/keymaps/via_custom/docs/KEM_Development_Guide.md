## Philosophy

The goal of this project is not only to build firmware.

The goal is to build a firmware that can continue to evolve safely.

Therefore, architecture, debugging, documentation and maintainability
are considered first-class deliverables alongside the source code.

このプロジェクトの成果は、ファームウェアだけではない。

安全に育て続けられる設計・デバッグ基盤・ドキュメントも成果物である。

# KEM Ver.2 Development Guide

Version: 1.0  
Date: 2026-07-09

## 1. Purpose

KEM Ver.2 is not only a Keyball44 firmware customization project.

It is a long-term development project focused on:

- maintainability
- extensibility
- observability
- small-step verification
- avoiding unnecessary rewrites

## 2. Development Style

This project follows:

1. Review
2. Design
3. Minimal implementation
4. Compile check
5. Real-device verification
6. Lessons learned

## 3. Core Principles

- Do not rush implementation.
- Separate observation from assumption.
- Use Debug Framework before guessing.
- Prefer small commits.
- Preserve responsibilities between modules.
- Avoid rewriting by designing before coding.
- Keep rules few and meaningful.
- Review Flash Budget when adding features.

## 4. Current Development Rule

When proposing code changes:

- Show the exact target file.
- Prefer “before → after”.
- Keep each change small.
- Ensure the shown change is compilable.
- Mention what to verify after compiling.

## 5. Current Project Status

Completed foundations:

- KEM Engine
- KEM LED Engine
- KEM Debug Framework
- Raw HID Debug Output
- Python Debug Viewer
- OLED Debug Page
- Debug ON/OFF toggle
- Matrix / Keycode / LED number visualization

Current Sprint:

- Analyze and fix Tap/Hold key LED behavior.

## 6. Debug Policy

Debug is OFF by default.

Shift + MENU toggles Debug ON/OFF.

When Debug is ON:

- Raw HID packets are sent.
- OLED DBG indicator is inverted.

OLED is used for quick local checks.  
Python Viewer is used for detailed inspection.

## 7. Flash Budget Policy

If firmware size becomes tight:

1. Remove unused QMK features first.
2. Review RGB effects and optional features.
3. Optimize custom code only after that.

Current known optimization:

- RGBLIGHT_EFFECT_STATIC_GRADIENT disabled
- EXTRAKEY_ENABLE = no
