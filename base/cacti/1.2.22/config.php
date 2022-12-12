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
 * When the cacti server is a remote poller, then these entries point to
 * the main cacti server. Otherwise, these variables have no use and
 * must remain commented out.
 */

#$rdatabase_type     = 'mysql';
#$rdatabase_default  = 'cacti';
#$rdatabase_hostname = 'localhost';
#$rdatabase_username = 'cactiuser';
#$rdatabase_password = 'cactiuser';
#$rdatabase_port     = '3306';
#$rdatabase_retries  = 5;
#$rdatabase_ssl      = false;
#$rdatabase_ssl_key  = '';
#$rdatabase_ssl_cert = '';
#$rdatabase_ssl_ca   = '';

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

/*
 * Optional parameters to define scripts and resource paths. These
 * variables become important when using remote poller installs when the
 * scripts and resource files are not in the main Cacti web server path.
 */

//$scripts_path = '/var/www/html/cacti/scripts';
//$resource_path = '/var/www/html/cacti/resource/';

/*
 * Optional parameter to define a data input whitelist command string. This
 * whitelist file will help protect cacti from unauthorized changes to Cacti
 * data input command string.
 */

//$input_whitelist = '/usr/local/etc/cacti/input_whitelist.json';

/*
 * Optional parameter to give explicit path to PHP
 */
//$php_path = '/bin/php';

/*
 * Optional parameter to disable the PHP SNMP extension. If not set, defaults
 * to class_exists('SNMP').
 */

//$php_snmp_support = false;

/*
 * Optional parameter to define the path of the csrf_secret.php path.  This
 * variable is for packagers who wish to specify an alternate location of
 * the CRSF secret file.
 */

//$path_csrf_secret = '/usr/share/cacti/resource/csrf-secret.php';

/*
 * The following are optional variables for debugging low level system
 * functions that are generally only used by Cacti Developers to help
 * identify potential issues in commonly used functions
 *
 * To use them, uncomment and the equivalent field will be set in the
 * $config variable allowing for instant on but still allowing the
 * ability to fine turn and turn them off.
 */

/*
 * Debug the read_config_option program flow
 */
# define('DEBUG_READ_CONFIG_OPTION', true);

/*
 * Automatically suppress the DEBUG_READ_CONFIG_OPTION
 */
# define('DEBUG_READ_CONFIG_OPTION_DB_OPEN', true);

/*
 * Always write the SQL command to the cacti log file
 */
# define('DEBUG_SQL_CMD', true);

/*
 * Debug the flow of calls to the db_xxx functions that
 * are defined in lib/database.php
 */
# define('DEBUG_SQL_FLOW', true);