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
