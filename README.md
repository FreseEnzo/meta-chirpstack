# Meta-ChirpStack Yocto Layer

## Overview

**Meta-ChirpStack** is a Yocto layer that seamlessly integrates the ChirpStack LoRaWAN Network Server and Gateway Bridge into your Yocto-based projects. This layer provides prebuilt binaries and configurations to simplify the deployment and management of ChirpStack components, enabling robust and scalable LoRaWAN networks.

## Table of Contents

- [Features](#features)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Integration](#integration)
  - [Option 1: Add the `meta-chirpstack` Layer and Modify `chirpstack.toml`](#option-1-add-the-meta-chirpstack-layer-and-modify-chirpstacktoml)
  - [Option 2: Use a Custom Meta-Layer to Overwrite `chirpstack.toml`](#option-2-use-a-custom-meta-layer-to-overwrite-chirpstacktoml)
  - [Generating a Secret Key](#generating-a-secret-key)
- [Configuration](#configuration)
  - [Version Overrides](#version-overrides)
  - [Source URIs and Checksums](#source-uris-and-checksums)
- [Usage](#usage)
  - [Starting Services](#starting-services)
  - [PostgreSQL Initialization](#postgresql-initialization)
- [Services](#services)
- [Dependencies](#dependencies)
- [Contributing](#contributing)
  - [How to Contribute](#how-to-contribute)
  - [Reporting Issues](#reporting-issues)
- [License](#license)
- [Support](#support)

## Features

- **Prebuilt Binaries:** Includes prebuilt binaries for ChirpStack Server and ChirpStack Gateway Bridge.
- **Systemd Integration:** Automatically configures systemd services for managing ChirpStack components.
- **PostgreSQL Initialization:** Sets up the ChirpStack role, database, and extensions in PostgreSQL.
- **Easy Configuration:** Provides default configuration files with the ability to customize as needed.
- **Yocto Compatibility:** Adheres to Yocto Project standards, ensuring smooth integration with your existing Yocto builds.

## Prerequisites

Before integrating the Meta-ChirpStack layer into your Yocto project, ensure that you have the following prerequisites:

- **Yocto Project:** A working Yocto build environment.
- **Supported Versions:**
  - **ChirpStack Server:** 4.9.0
  - **ChirpStack Gateway Bridge:** 4.0.9
  - **PostgreSQL:** Compatible version as required by ChirpStack
  - **Mosquitto:** MQTT broker
  - **Redis:** In-memory data structure store

## Installation

Follow these steps to add the Meta-ChirpStack layer to your Yocto project:

1. **Clone the Repository:**

   ```bash
   git clone https://github.com/FreseEnzo/meta-chirpstack.git
   ```

2. **Add the Layer to Your Build Configuration:**

   Navigate to your Yocto build directory and add the layer using `bitbake-layers`:

   ```bash
   bitbake-layers add-layer ../path-to/meta-chirpstack
   ```

3. **Set Layer Priorities (Optional):**

   If necessary, adjust the layer priorities in your `bblayers.conf` to resolve any potential conflicts.

## Integration

To integrate ChirpStack into your project, you need to add a `secret = "<YOUR SECRET KEY>"` entry to the `chirpstack.toml` configuration file located at `recipes-chirpstack/chirpstack/files/chirpstack.toml`. You can achieve this in two ways:

### Option 1: Add the `meta-chirpstack` Layer and Modify `chirpstack.toml`

1. **Add the `meta-chirpstack` Layer:**
   - Incorporate the `meta-chirpstack` layer into your project’s build environment (already covered in the [Installation](#installation) section).

2. **Modify the `chirpstack.toml` File:**
   - Open the `chirpstack.toml` file located at `recipes-chirpstack/chirpstack/files/chirpstack.toml`.
   - Add or update the `secret` key as shown below:

   ```toml
   # Secret.
   #
   # This secret is used for generating login and API tokens. Ensure this
   # is never exposed. Changing this secret will invalidate all login and API
   # tokens. You can generate a random secret using the following command:
   #   openssl rand -base64 32
   secret = "<YOUR SECRET KEY>"
   ```

### Option 2: Use a Custom Meta-Layer to Overwrite `chirpstack.toml`

1. **Create the Necessary Directory Structure:**
   - Create the following directories and file in your custom meta-layer:

     ```
     your-meta-layer/
     └── recipes-chirpstack/
         └── chirpstack/
             └── chirpstack_%.bb
     ```

2. **Configure the BitBake Recipe:**
   - Open `chirpstack_%.bb` and add the following content:

     ```bitbake
     FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

     do_install:append() {
         install -m 0644 ${WORKDIR}/chirpstack.toml ${D}${sysconfdir}/chirpstack/chirpstack.toml
     }

     INSANE_SKIP:${PN} += "already-stripped"
     ```

3. **Add the Custom `chirpstack.toml` File:**
   - Create the `files` directory:

     ```
     your-meta-layer/
     └── recipes-chirpstack/
         └── chirpstack/
             └── files/
                 └── chirpstack.toml
     ```

   - Copy your customized `chirpstack.toml` into the `files` directory and modify it as needed.

   **Tip:** This approach will overwrite the default `chirpstack.toml` with your custom configuration.

4. **Update `layer.conf`:**
   - Modify your `layer.conf` file to include ChirpStack in the image:

     ```bitbake
     IMAGE_INSTALL:append = " chirpstack"
     ```

### Generating a Secret Key

Regardless of the option you choose, ensure you generate a secure secret key. You can generate a random secret using the following command:

```bash
openssl rand -base64 32
```

## Configuration

The Meta-ChirpStack layer provides default configurations, which you can customize according to your requirements.

### Version Overrides

You can override the default ChirpStack versions by defining them in your recipe `.bb` files:

```bash
CHIRPSTACK_VERSION ?= "4.9.0"
CHIRPSTACK_GW_BRIDGE_VERSION ?= "4.0.9"
```

### Source URIs and Checksums

The layer fetches prebuilt binaries from ChirpStack's official repositories. Ensure that the checksums match the downloaded files for integrity:

```bash
CHIRPSTACK_SERVER_SHA256 = "1d07a1701290b103e58eb570d19cbdf39e0009a630c4b0364002e406fbb22423"
CHIRPSTACK_GW_BRIDGE_SHA256 = "2bb41ba0b9c451cf84473b7ca489b2ec62912fd8974544cb96667bae571b8815"
```

## Usage

Once installed and configured, the ChirpStack components can be managed using systemd services.

### Starting Services

Enable and start the ChirpStack Server and Gateway Bridge services:

```bash
sudo systemctl enable chirpstack
sudo systemctl start chirpstack

sudo systemctl enable chirpstack-gateway-bridge
sudo systemctl start chirpstack-gateway-bridge
```

### PostgreSQL Initialization

The layer includes scripts and services to initialize the ChirpStack PostgreSQL database:

```bash
sudo systemctl enable chirpstack-postgresql-db
sudo systemctl start chirpstack-postgresql-db
```

## Services

The Meta-ChirpStack layer sets up the following systemd services:

- **ChirpStack Server:**
  - **Service File:** `chirpstack.service`
  - **Configuration File:** `chirpstack.toml`

- **ChirpStack Gateway Bridge:**
  - **Service File:** `chirpstack-gateway-bridge.service`
  - **Configuration File:** `chirpstack-gateway-bridge.toml`
  - **Region Configuration:** `region_eu868.toml`

- **PostgreSQL Database Initialization:**
  - **Service File:** `chirpstack-postgresql-db.service`
  - **Script:** `chirpstack-postgresql-db.sh`

These services are automatically enabled and started upon installation.

## Dependencies

Ensure that the following dependencies are included in your Yocto build for the proper functioning of ChirpStack components:

- **Mosquitto:** MQTT broker
- **Redis:** In-memory data structure store
- **PostgreSQL:** Database server

These dependencies are automatically handled by the Meta-ChirpStack layer:

```bash
RDEPENDS_${PN} += "mosquitto redis postgresql"
```

## Contributing

We welcome contributions to the Meta-ChirpStack layer! Whether you're fixing bugs, improving documentation, or adding new features, your input is valuable.

### How to Contribute

1. **Fork the Repository:**

   Click the "Fork" button at the top right of this page to create your own fork.

2. **Clone Your Fork:**

   ```bash
   git clone https://github.com/FreseEnzo/meta-chirpstack.git
   ```

3. **Create a New Branch:**

   ```bash
   git checkout -b feature/YourFeatureName
   ```

4. **Make Your Changes:**

   Implement your feature or fix in the appropriate files.

5. **Commit Your Changes:**

   ```bash
   git commit -m "Add feature: YourFeatureName"
   ```

6. **Push to Your Fork:**

   ```bash
   git push origin feature/YourFeatureName
   ```

7. **Submit a Pull Request:**

   Navigate to the original repository and click "Compare & pull request."

### Reporting Issues

If you encounter any issues or have suggestions for improvements, please [open an issue](https://github.com/FreseEnzo/meta-chirpstack/issues) on GitHub.

## License

**Meta-ChirpStack Yocto Layer** is licensed under the **MIT** license. Please refer to the [LICENSE](LICENSE) file for more details.

## Support

For support and further inquiries, please reach out via the [ChirpStack Community](https://www.chirpstack.io/community/) or open an issue on the [GitHub repository](https://github.com/FreseEnzo/meta-chirpstack/issues).

---

*This project is maintained by [Enzo Frese](https://github.com/FreseEnzo). Thank you for using Meta-ChirpStack!*