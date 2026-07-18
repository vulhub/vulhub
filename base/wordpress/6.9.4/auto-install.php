<?php
/**
 * Non-interactively install WordPress on first boot (Vulhub lab convenience).
 *
 * Exits non-zero while the database is still unreachable so the entrypoint keeps
 * retrying, and exits zero once WordPress is installed. Creates an
 * administrator "admin" / "admin" together with the default "Hello world!" post,
 * which the wp2shell exploit relies on.
 */
define( 'WP_INSTALLING', true );

// wp_install() guesses the site URL from HTTP_HOST, which is unavailable in
// this CLI process. Use a stable placeholder; the dynamic-siteurl MU plugin
// replaces it with the request host when the lab is accessed over HTTP.
define( 'WP_SITEURL', 'http://localhost' );

$host = getenv( 'WORDPRESS_DB_HOST' ) ?: 'db';
$port = 3306;
if ( strpos( $host, ':' ) !== false ) {
	list( $host, $port ) = explode( ':', $host, 2 );
}
$user = getenv( 'WORDPRESS_DB_USER' ) ?: 'root';
$pass = getenv( 'WORDPRESS_DB_PASSWORD' ) ?: '';
$name = getenv( 'WORDPRESS_DB_NAME' ) ?: 'wordpress';

// Bail out (retry later) until the database accepts connections.
mysqli_report( MYSQLI_REPORT_OFF );
$conn = @mysqli_connect( $host, $user, $pass, $name, (int) $port );
if ( ! $conn ) {
	fwrite( STDERR, "[auto-install] database not ready yet\n" );
	exit( 1 );
}
mysqli_close( $conn );

require '/var/www/html/wp-load.php';
require_once ABSPATH . 'wp-admin/includes/upgrade.php';

if ( is_blog_installed() ) {
	exit( 0 );
}

wp_install( 'wp2shell', 'admin', 'admin@example.com', true, '', 'admin' );
update_option( 'siteurl', WP_SITEURL );
update_option( 'home', WP_SITEURL );
fwrite( STDERR, "[auto-install] WordPress installed\n" );
