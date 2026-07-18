<?php
/**
 * Plugin Name: Vulhub Dynamic Site URL
 * Description: Serve siteurl/home from the current request host so the lab works
 *              on any IP or port without editing the database.
 */
if ( ! defined( 'ABSPATH' ) ) {
	exit;
}

function vulhub_dynamic_site_url( $value ) {
	if ( ! empty( $_SERVER['HTTP_HOST'] ) ) {
		$scheme = ( ! empty( $_SERVER['HTTPS'] ) && 'off' !== $_SERVER['HTTPS'] ) ? 'https' : 'http';
		return $scheme . '://' . $_SERVER['HTTP_HOST'];
	}
	return $value;
}

add_filter( 'option_home', 'vulhub_dynamic_site_url' );
add_filter( 'option_siteurl', 'vulhub_dynamic_site_url' );
