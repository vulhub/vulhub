<?php
/*
 +-------------------------------------------------------------------------+
 | Copyright (C) 2004-2020 The Cacti Group                                 |
 |                                                                         |
 | This program is free software; you can redistribute it and/or           |
 | modify it under the terms of the GNU General Public License             |
 | as published by the Free Software Foundation; either version 2          |
 | of the License, or (at your option) any later version.                  |
 |                                                                         |
 | This program is distributed in the hope that it will be useful,         |
 | but WITHOUT ANY WARRANTY; without even the implied warranty of          |
 | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           |
 | GNU General Public License for more details.                            |
 +-------------------------------------------------------------------------+
 | Cacti: The Complete RRDtool-based Graphing Solution                     |
 +-------------------------------------------------------------------------+
 | This code is designed, written, and maintained by the Cacti Group. See  |
 | about.php and/or the AUTHORS file for specific developer information.   |
 +-------------------------------------------------------------------------+
 | http://www.cacti.net/                                                   |
 +-------------------------------------------------------------------------+
*/

/*
 * Make sure these values reflect your actual database/host/user/password
 */

$database_type     = 'mysql';
$database_default  = 'cacti';
$database_hostname = 'db';
$database_username = 'root';
$database_password = 'root';
$database_port     = '3306';
$database_retries  = 5;
$database_ssl      = false;
$database_ssl_key  = '';
$database_ssl_cert = '';
$database_ssl_ca   = '';
$database_persist  = false;

/*
 * The poller_id of this system.  set to `1` for the main cacti web server.
 * Otherwise, you this value should be the poller_id for the remote poller.
 */

$poller_id = 1;

/*
 * Set the $url_path to point to the default URL of your cacti install.
 * For example if your cacti install as at `https://serverip/cacti/` this
 * would be set to `/cacti/`.
 */

$url_path = '/';

/*
 * Default session name - session name must contain alpha characters
 */

$cacti_session_name = 'Cacti';

/*
 * Default Cookie domain - The cookie domain to be used for Cacti
 */

//$cacti_cookie_domain = 'cacti.net';

/*
 * Save sessions to a database for load balancing
 */

$cacti_db_session = false;

/*
 * Disable log rotation settings for packagers
 */

$disable_log_rotation = false;

/**
 * Allow the use of Proxy IPs when searching for client
 * IP to be used
 *
 * This can be set to one of the following:
 *   - false: to use only REMOTE_ADDR
 *   - true: to use all allowed headers (not advised)
 *   - array of one or more the following:
 *		'X-Forwarded-For',
 *		'X-Client-IP',
 *		'X-Real-IP',
 *		'X-ProxyUser-Ip',
 *		'CF-Connecting-IP',
 *		'True-Client-IP',
 *		'HTTP_X_FORWARDED',
 *		'HTTP_X_FORWARDED_FOR',
 *		'HTTP_X_CLUSTER_CLIENT_IP',
 *		'HTTP_FORWARDED_FOR',
 *		'HTTP_FORWARDED',
 *		'HTTP_CLIENT_IP',
 *
 * NOTE: The following will always be checked:
 *		'REMOTE_ADDR',
 */
$proxy_headers = null;

/*
 * Set this to define the default i18n handler if not set
 * in the database.  For valid values, see CACTI_LANGUAGE_HANDLER
 * constants but only use the value not the constant name
 * here because we have not included global_constants.php
 * yet
 */
$i18n_handler = null;

/*
 * Set this to define ignore everything else and force a
 * specific language.  Should be in the format of a string
 * such as 'es-ES'
 */
$i18n_force_language = null;

/**
 * Define the location to log general i18n function calls
 * to for debugging purposes.  Note that if you use /tmp
 * some systemd systems will remap that folder to special
 * locations under /tmp/systemd<uniqueid>/tmp
 */
$i18n_log = null;

/**
 * Define the location to log translation i18n function calls
 * to for debugging purposes.  Note that if you use /tmp
 * some systemd systems will remap that folder to special
 * locations under /tmp/systemd<uniqueid>/tmp
 */
$i18n_text_log = null;

