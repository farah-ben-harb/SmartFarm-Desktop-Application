# SmartFarm Desktop Application

SmartFarm is a desktop application for agricultural management built with C++ and Qt, with Arduino integration for basic automation workflows.

## Overview

The project is designed to help manage:

- stock
- sales
- suppliers
- products
- equipment
- simple automation scenarios

## Tech Stack

- C++
- Qt
- Arduino

## Project Structure

- `main.cpp` application entry point
- `mainwindow.*` main UI logic
- `gestion_fournisseurs.*` supplier management
- `fournisseur.*` supplier model and helpers
- `affichage.*` display views
- `evenement.*` event management
- `connexion.*` connection logic
- `suppevent.*` and `modifevent.*` event actions

## Features

- Product and stock management
- Sales and supplier tracking
- GUI built with Qt Designer
- Arduino-based automation examples
- Agricultural event management

## Getting Started

1. Open `Integration.pro` in Qt Creator.
2. Configure the Qt kit and compiler for your system.
3. Build the project.
4. Run the application and verify the relevant UI screens.

## Branching and Commits

- Use branches such as `feat/<area>`, `fix/<area>`, `docs/<area>`, or `ui/<area>`
- Keep commits focused on one UI or data model change
- Prefer Conventional Commits such as `feat(farm): add supplier search dialog`
- Avoid vague messages like `update`, `final`, or `test`

## Contributing

- Keep UI and hardware changes easy to review
- Document any Arduino wiring assumption or hardware dependency
- Avoid committing generated binaries or local build output
