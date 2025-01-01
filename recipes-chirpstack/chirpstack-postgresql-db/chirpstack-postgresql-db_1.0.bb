SUMMARY = "ChirpStack PostgreSQL initialization"
DESCRIPTION = "Sets up the ChirpStack role, database, and extensions in PostgreSQL."
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "\
    file://chirpstack-postgresql-db.sh \
    file://chirpstack-postgresql-db.service \
"

inherit systemd

# We need PostgreSQL at runtime:
RDEPENDS:${PN} = "postgresql"

S = "${WORKDIR}"

do_compile() {
    :
}

do_install() {
    # Install the shell script
    install -d ${D}${sbindir}
    install -m 0755 ${WORKDIR}/chirpstack-postgresql-db.sh \
        ${D}${sbindir}/chirpstack-postgresql-db.sh

    # Install the systemd service
    install -d ${D}${systemd_unitdir}/system
    install -m 0644 ${WORKDIR}/chirpstack-postgresql-db.service \
        ${D}${systemd_unitdir}/system
}

FILES:${PN} += " \
    ${sbindir}/chirpstack-postgresql-db.sh \
    ${systemd_unitdir}/system/chirpstack-postgresql-db.service \
"

SYSTEMD_SERVICE:${PN} = "chirpstack-postgresql-db.service"
SYSTEMD_AUTO_ENABLE = "enable"
