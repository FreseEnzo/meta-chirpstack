SUMMARY ?= "ChirpStack (Network Server + Gateway Bridge) - Prebuilt Binaries"
DESCRIPTION ?= "Install and configure ChirpStack LoRaWAN Network Server and Gateway Bridge."
HOMEPAGE = "https://www.chirpstack.io/"
LICENSE = "CLOSED"

# If you want to override versions in .bb, define them there.
# These can be set as default values here in .inc for convenience.
CHIRPSTACK_VERSION ?= "4.9.0"
CHIRPSTACK_GW_BRIDGE_VERSION ?= "4.0.9"

# Example placeholders for checksums (overridden in .bb).
CHIRPSTACK_SERVER_SHA256 ?= ""
CHIRPSTACK_GW_BRIDGE_SHA256 ?= ""

SRC_URI = "\
    https://artifacts.chirpstack.io/downloads/chirpstack/chirpstack_${CHIRPSTACK_VERSION}_linux_arm64.tar.gz;subdir=${BPN}-server-${CHIRPSTACK_VERSION};name=server \
    https://artifacts.chirpstack.io/downloads/chirpstack-gateway-bridge/chirpstack-gateway-bridge_${CHIRPSTACK_GW_BRIDGE_VERSION}_linux_arm64.tar.gz;subdir=${BPN}-gateway-bridge-${CHIRPSTACK_GW_BRIDGE_VERSION};name=bridge \
    file://chirpstack.service \
    file://chirpstack.toml \
    file://chirpstack-gateway-bridge.service \
    file://chirpstack-gateway-bridge.toml \
    file://region_eu868.toml \
"

SRC_URI[server.sha256sum] ?= "${CHIRPSTACK_SERVER_SHA256}"
SRC_URI[bridge.sha256sum] ?= "${CHIRPSTACK_GW_BRIDGE_SHA256}"

S = "${WORKDIR}"

inherit systemd

SYSTEMD_PACKAGES = "${PN}"
SYSTEMD_SERVICE:${PN} = "chirpstack.service chirpstack-gateway-bridge.service"

# Dependencies for runtime
RDEPENDS:${PN} += " \
    mosquitto \
    redis \
    postgresql \
"

# No-op compile since we are installing prebuilt binaries
do_compile() {
    :
}

do_install() {
    # Create directories
    install -d ${D}${bindir}
    install -d ${D}${sysconfdir}/chirpstack
    install -d ${D}${sysconfdir}/chirpstack-gateway-bridge
    install -d ${D}${systemd_unitdir}/system

    #
    # 1) Install ChirpStack server
    #
    install -m 0755 ${S}/${BPN}-server-${CHIRPSTACK_VERSION}/chirpstack \
        ${D}${bindir}/chirpstack

    install -m 0644 ${WORKDIR}/chirpstack.toml \
        ${D}${sysconfdir}/chirpstack/chirpstack.toml

    install -m 0644 ${WORKDIR}/chirpstack.service \
        ${D}${systemd_unitdir}/system/chirpstack.service

    #
    # 2) Install ChirpStack Gateway Bridge
    #
    install -m 0755 ${S}/${BPN}-gateway-bridge-${CHIRPSTACK_GW_BRIDGE_VERSION}/chirpstack-gateway-bridge \
        ${D}${bindir}/chirpstack-gateway-bridge

    install -m 0644 ${WORKDIR}/chirpstack-gateway-bridge.toml \
        ${D}${sysconfdir}/chirpstack-gateway-bridge/chirpstack-gateway-bridge.toml

    install -m 0644 ${WORKDIR}/chirpstack-gateway-bridge.service \
        ${D}${systemd_unitdir}/system/chirpstack-gateway-bridge.service

    # Region config
    install -m 0644 ${WORKDIR}/region_eu868.toml \
        ${D}${sysconfdir}/chirpstack/region_eu868.toml
}

SYSTEMD_AUTO_ENABLE = "enable"
