/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements. See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- database for Manager Web
-- ----------------------------
CREATE DATABASE IF NOT EXISTS apache_inlong_manager;
USE apache_inlong_manager;

-- ----------------------------
-- Table structure for inlong_group
-- ----------------------------
CREATE TABLE IF NOT EXISTS `inlong_group`
(
    `id`                     int(11)      NOT NULL AUTO_INCREMENT COMMENT 'Incremental primary key',
    `inlong_group_id`        varchar(256) NOT NULL COMMENT 'Inlong group id, filled in by the user, undeleted ones cannot be repeated',
    `name`                   varchar(128)          DEFAULT '' COMMENT 'Inlong group name, English, Chinese, numbers, etc',
    `description`            varchar(256)          DEFAULT '' COMMENT 'Description of inlong group',
    `mq_type`                varchar(20)           DEFAULT 'TUBEMQ' COMMENT 'The message queue type, high throughput: TUBEMQ, high consistency: PULSAR',
    `mq_resource`            varchar(128) NOT NULL COMMENT 'MQ resource, for TubeMQ, its Topic, for Pulsar, its Namespace',
    `daily_records`          int(11)               DEFAULT '10' COMMENT 'Number of access records per day, unit: 10,000 records per day',
    `daily_storage`          int(11)               DEFAULT '10' COMMENT 'Access size by day, unit: GB per day',
    `peak_records`           int(11)               DEFAULT '1000' COMMENT 'Access peak per second, unit: records per second',
    `max_length`             int(11)               DEFAULT '10240' COMMENT 'The maximum length of a single piece of data, unit: Byte',
    `enable_zookeeper`       tinyint(1)            DEFAULT '0' COMMENT 'Whether to enable the zookeeper, 0-disable, 1-enable',
    `enable_create_resource` tinyint(1)            DEFAULT '1' COMMENT 'Whether to enable create resource? 0-disable, 1-enable',
    `lightweight`            tinyint(1)            DEFAULT '0' COMMENT 'Whether to use lightweight mode, 0-no, 1-yes',
    `inlong_cluster_tag`     varchar(128)          DEFAULT NULL COMMENT 'The cluster tag, which links to inlong_cluster table',
    `ext_params`             text                  DEFAULT NULL COMMENT 'Extended params, will be saved as JSON string, such as queue_module, partition_num',
    `in_charges`             varchar(512) NOT NULL COMMENT 'Name of responsible person, separated by commas',
    `followers`              varchar(512)          DEFAULT NULL COMMENT 'Name of followers, separated by commas',
    `status`                 int(4)                DEFAULT '100' COMMENT 'Inlong group status',
    `previous_status`        int(4)                DEFAULT '100' COMMENT 'Previous group status',
    `is_deleted`             int(11)               DEFAULT '0' COMMENT 'Whether to delete, 0: not deleted, > 0: deleted',
    `creator`                varchar(64)  NOT NULL COMMENT 'Creator name',
    `modifier`               varchar(64)           DEFAULT NULL COMMENT 'Modifier name',
    `create_time`            timestamp    NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Create time',
    `modify_time`            timestamp    NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT 'Modify time',
    `version`                int(11)      NOT NULL DEFAULT '1' COMMENT 'Version number, which will be incremented by 1 after modification',
    PRIMARY KEY (`id`),
    UNIQUE KEY `unique_inlong_group` (`inlong_group_id`, `is_deleted`),
    INDEX group_status_deleted_index (`status`, `is_deleted`)
) ENGINE = InnoDB
  DEFAULT CHARSET = utf8mb4 COMMENT ='Inlong group table';

-- ----------------------------
-- Table structure for inlong_group_ext
-- ----------------------------
CREATE TABLE IF NOT EXISTS `inlong_group_ext`
(
    `id`              int(11)      NOT NULL AUTO_INCREMENT COMMENT 'Incremental primary key',
    `inlong_group_id` varchar(256) NOT NULL COMMENT 'Inlong group id',
    `key_name`        varchar(256) NOT NULL COMMENT 'Configuration item name',
    `key_value`       text              DEFAULT NULL COMMENT 'The value of the configuration item',
    `is_deleted`      int(11)           DEFAULT '0' COMMENT 'Whether to delete, 0: not deleted, > 0: deleted',
    `modify_time`     timestamp    NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT 'Modify time',
    PRIMARY KEY (`id`),
    KEY `group_id_index` (`inlong_group_id`),
    UNIQUE KEY `unique_inlong_group_key` (`inlong_group_id`, `key_name`)
) ENGINE = InnoDB
  DEFAULT CHARSET = utf8mb4 COMMENT ='Inlong group extension table';

-- ----------------------------
-- Table structure for inlong_cluster_tag
-- ----------------------------
CREATE TABLE IF NOT EXISTS `inlong_cluster_tag`
(
    `id`          int(11)      NOT NULL AUTO_INCREMENT COMMENT 'Incremental primary key',
    `cluster_tag` varchar(128) NOT NULL COMMENT 'Cluster tag',
    `ext_params`  text                  DEFAULT NULL COMMENT 'Extended params, will be saved as JSON string',
    `description` varchar(256)          DEFAULT '' COMMENT 'Description of cluster tag',
    `in_charges`  varchar(512) NOT NULL COMMENT 'Name of responsible person, separated by commas',
    `status`      int(4)                DEFAULT '0' COMMENT 'Cluster status',
    `is_deleted`  int(11)               DEFAULT '0' COMMENT 'Whether to delete, 0: not deleted, > 0: deleted',
    `creator`     varchar(64)  NOT NULL COMMENT 'Creator name',
    `modifier`    varchar(64)           DEFAULT NULL COMMENT 'Modifier name',
    `create_time` timestamp    NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Create time',
    `modify_time` timestamp    NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT 'Modify time',
    `version`     int(11)      NOT NULL DEFAULT '1' COMMENT 'Version number, which will be incremented by 1 after modification',
    PRIMARY KEY (`id`),
    UNIQUE KEY `unique_inlong_cluster_tag` (`cluster_tag`, `is_deleted`)
) ENGINE = InnoDB
  DEFAULT CHARSET = utf8mb4 COMMENT ='Inlong cluster tag table';

-- ----------------------------
-- Table structure for inlong_cluster
-- ----------------------------
CREATE TABLE IF NOT EXISTS `inlong_cluster`
(
    `id`           int(11)      NOT NULL AUTO_INCREMENT COMMENT 'Incremental primary key',
    `name`         varchar(128) NOT NULL COMMENT 'Cluster name',
    `type`         varchar(20)           DEFAULT '' COMMENT 'Cluster type, such as: TUBEMQ, PULSAR, DATAPROXY, etc',
    `url`          varchar(512)          DEFAULT NULL COMMENT 'Cluster URL',
    `cluster_tags` varchar(512)          DEFAULT NULL COMMENT 'Cluster tag, separated by commas',
    `ext_tag`      varchar(128)          DEFAULT NULL COMMENT 'Extension tag, for extended use',
    `token`        varchar(512)          DEFAULT NULL COMMENT 'Cluster token',
    `ext_params`   text                  DEFAULT NULL COMMENT 'Extended params, will be saved as JSON string',
    `description`  varchar(256)          DEFAULT '' COMMENT 'Description of cluster',
    `heartbeat`    text                  DEFAULT NULL COMMENT 'Cluster heartbeat info',
    `in_charges`   varchar(512) NOT NULL COMMENT 'Name of responsible person, separated by commas',
    `status`       int(4)                DEFAULT '0' COMMENT 'Cluster status',
    `is_deleted`   int(11)               DEFAULT '0' COMMENT 'Whether to delete, 0: not deleted, > 0: deleted',
    `creator`      varchar(64)  NOT NULL COMMENT 'Creator name',
    `modifier`     varchar(64)           DEFAULT NULL COMMENT 'Modifier name',
    `create_time`  timestamp    NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Create time',
    `modify_time`  timestamp    NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT 'Modify time',
    `version`      int(11)      NOT NULL DEFAULT '1' COMMENT 'Version number, which will be incremented by 1 after modification',
    PRIMARY KEY (`id`),
    UNIQUE KEY `unique_inlong_cluster` (`name`, `type`, `is_deleted`)
) ENGINE = InnoDB
  DEFAULT CHARSET = utf8mb4 COMMENT ='Inlong cluster table';

-- ----------------------------
-- Table structure for inlong_cluster_node
-- ----------------------------
CREATE TABLE IF NOT EXISTS `inlong_cluster_node`
(
    `id`          int(11)      NOT NULL AUTO_INCREMENT COMMENT 'Incremental primary key',
    `parent_id`   int(11)      NOT NULL COMMENT 'Id of the parent cluster',
    `type`        varchar(20)  NOT NULL COMMENT 'Cluster type, such as: AGENT, DATAPROXY, etc',
    `ip`          varchar(512) NOT NULL COMMENT 'Cluster IP, separated by commas, such as: 127.0.0.1:8080,host2:8081',
    `port`        int(6)       NULL COMMENT 'Cluster port',
    `ext_params`  text                  DEFAULT NULL COMMENT 'Another fields will be saved as JSON string',
    `description` varchar(256)          DEFAULT '' COMMENT 'Description of cluster node',
    `status`      int(4)                DEFAULT '0' COMMENT 'Cluster status',
    `is_deleted`  int(11)               DEFAULT '0' COMMENT 'Whether to delete, 0: not deleted, > 0: deleted',
    `creator`     varchar(64)  NOT NULL COMMENT 'Creator name',
    `modifier`    varchar(64)           DEFAULT NULL COMMENT 'Modifier name',
    `create_time` timestamp    NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Create time',
    `modify_time` timestamp    NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT 'Modify time',
    `version`     int(11)      NOT NULL DEFAULT '1' COMMENT 'Version number, which will be incremented by 1 after modification',
    PRIMARY KEY (`id`),
    UNIQUE KEY `unique_inlong_cluster_node` (`parent_id`, `type`, `ip`, `port`, `is_deleted`)
) ENGINE = InnoDB
  DEFAULT CHARSET = utf8mb4 COMMENT ='Inlong cluster node table';

-- ----------------------------
-- Table structure for data_node
-- ----------------------------
CREATE TABLE IF NOT EXISTS `data_node`
(
    `id`          int(11)      NOT NULL AUTO_INCREMENT COMMENT 'Incremental primary key',
    `name`        varchar(128) NOT NULL COMMENT 'Node name',
    `type`        varchar(20)           DEFAULT '' COMMENT 'Node type, such as: MYSQL, HIVE, KAFKA, ES, etc',
    `url`         varchar(512)          DEFAULT NULL COMMENT 'Node URL',
    `username`    varchar(128)          DEFAULT NULL COMMENT 'Username for node if needed',
    `token`       varchar(512)          DEFAULT NULL COMMENT 'Node token',
    `ext_params`  text                  DEFAULT NULL COMMENT 'Extended params, will be saved as JSON string',
    `description` varchar(256)          DEFAULT '' COMMENT 'Description of data node',
    `in_charges`  varchar(512) NOT NULL COMMENT 'Name of responsible person, separated by commas',
    `status`      int(4)                DEFAULT '0' COMMENT 'Node status',
    `is_deleted`  int(11)               DEFAULT '0' COMMENT 'Whether to delete, 0: not deleted, > 0: deleted',
    `creator`     varchar(64)  NOT NULL COMMENT 'Creator name',
    `modifier`    varchar(64)           DEFAULT NULL COMMENT 'Modifier name',
    `create_time` timestamp    NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Create time',
    `modify_time` timestamp    NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT 'Modify time',
    `version`     int(11)      NOT NULL DEFAULT '1' COMMENT 'Version number, which will be incremented by 1 after modification',
    PRIMARY KEY (`id`),
    UNIQUE KEY `unique_data_node` (`name`, `type`, `is_deleted`)
) ENGINE = InnoDB
  DEFAULT CHARSET = utf8mb4 COMMENT ='Data node table';

-- ----------------------------
-- Table structure for consumption
-- ----------------------------
CREATE TABLE IF NOT EXISTS `consumption`
(
    `id`               int(11)      NOT NULL AUTO_INCREMENT COMMENT 'Incremental primary key',
    `consumer_group`   varchar(256) NOT NULL COMMENT 'Consumer group',
    `in_charges`       varchar(512) NOT NULL COMMENT 'Person in charge of consumption',
    `inlong_group_id`  varchar(256) NOT NULL COMMENT 'Inlong group id',
    `mq_type`          varchar(10)           DEFAULT 'TUBE' COMMENT 'Message queue type, high throughput: TUBE, high consistency: PULSAR',
    `topic`            varchar(256) NOT NULL COMMENT 'Consumption topic',
    `filter_enabled`   int(2)                DEFAULT '0' COMMENT 'Whether to filter, default 0, not filter consume',
    `inlong_stream_id` varchar(256)          DEFAULT NULL COMMENT 'Inlong stream ID for consumption, if filter_enable is 1, it cannot empty',
    `status`           int(4)       NOT NULL COMMENT 'Status: draft, pending approval, approval rejected, approval passed',
    `is_deleted`       int(11)               DEFAULT '0' COMMENT 'Whether to delete, 0: not deleted, > 0: deleted',
    `creator`          varchar(64)  NOT NULL COMMENT 'creator',
    `modifier`         varchar(64)           DEFAULT NULL COMMENT 'modifier',
    `create_time`      timestamp    NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Create time',
    `modify_time`      timestamp    NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT 'Modify time',
    `version`          int(11)      NOT NULL DEFAULT '1' COMMENT 'Version number, which will be incremented by 1 after modification',
    PRIMARY KEY (`id`)
) ENGINE = InnoDB
  DEFAULT CHARSET = utf8mb4 COMMENT ='Data consumption configuration table';

-- ----------------------------
-- Table structure for consumption_pulsar
-- ----------------------------
CREATE TABLE IF NOT EXISTS `consumption_pulsar`
(
    `id`                 int(11)      NOT NULL AUTO_INCREMENT,
    `consumption_id`     int(11)      DEFAULT NULL COMMENT 'ID of the consumption information to which it belongs, guaranteed to be uniquely associated with consumption information',
    `consumer_group`     varchar(256) NOT NULL COMMENT 'Consumer group',
    `inlong_group_id`    varchar(256) NOT NULL COMMENT 'Inlong group ID',
    `is_rlq`             tinyint(1)   DEFAULT '0' COMMENT 'Whether to configure the retry letter topic, 0: no configuration, 1: configuration',
    `retry_letter_topic` varchar(256) DEFAULT NULL COMMENT 'The name of the retry queue topic',
    `is_dlq`             tinyint(1)   DEFAULT '0' COMMENT 'Whether to configure dead letter topic, 0: no configuration, 1: means configuration',
    `dead_letter_topic`  varchar(256) DEFAULT NULL COMMENT 'dead letter topic name',
    `is_deleted`         int(11)      DEFAULT '0' COMMENT 'Whether to delete, 0: not deleted, > 0: deleted',
    PRIMARY KEY (`id`)
) ENGINE = InnoDB
  DEFAULT CHARSET = utf8mb4 COMMENT ='Pulsar consumption table';

-- ----------------------------
-- Table structure for stream_source_cmd_config
-- ----------------------------
CREATE TABLE IF NOT EXISTS `stream_source_cmd_config`
(
    `id`                  int(11)     NOT NULL AUTO_INCREMENT COMMENT 'cmd id',
    `cmd_type`            int(11)     NOT NULL,
    `task_id`             int(11)     NOT NULL,
    `specified_data_time` varchar(64) NOT NULL,
    `bSend`               tinyint(1)  NOT NULL,
    `create_time`         timestamp   NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Create time',
    `modify_time`         timestamp   NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT 'Modify time',
    `result_info`         varchar(64)      DEFAULT NULL,
    PRIMARY KEY (`id`),
    KEY `unique_source_cmd_config` (`task_id`, `bSend`, `specified_data_time`)
) ENGINE = InnoDB
  DEFAULT CHARSET = utf8;

-- ----------------------------
-- Table structure for inlong_stream
-- ----------------------------
CREATE TABLE IF NOT EXISTS `inlong_stream`
(
    `id`               int(11)      NOT NULL AUTO_INCREMENT COMMENT 'Incremental primary key',
    `inlong_group_id`  varchar(256) NOT NULL COMMENT 'Owning inlong group id',
    `inlong_stream_id` varchar(256) NOT NULL COMMENT 'Inlong stream id, non-deleted globally unique',
    `name`             varchar(64)           DEFAULT NULL COMMENT 'The name of the inlong stream page display, can be Chinese',
    `description`      varchar(256)          DEFAULT '' COMMENT 'Description of inlong stream',
    `mq_resource`      varchar(128)          DEFAULT NULL COMMENT 'MQ resource, in one stream, corresponding to the filter ID of TubeMQ, corresponding to the topic of Pulsar',
    `data_type`        varchar(20)           DEFAULT NULL COMMENT 'Data type, including: CSV, KEY-VALUE, JSON, AVRO, etc.',
    `data_encoding`    varchar(8)            DEFAULT 'UTF-8' COMMENT 'Data encoding format, including: UTF-8, GBK, etc.',
    `data_separator`   varchar(8)            DEFAULT NULL COMMENT 'The source data field separator',
    `data_escape_char` varchar(8)            DEFAULT NULL COMMENT 'Source data field escape character, the default is NULL (NULL), stored as 1 character',
    `sync_send`        tinyint(1)            DEFAULT '0' COMMENT 'order_preserving 0: none, 1: yes',
    `daily_records`    int(11)               DEFAULT '10' COMMENT 'Number of access records per day, unit: 10,000 records per day',
    `daily_storage`    int(11)               DEFAULT '10' COMMENT 'Access size by day, unit: GB per day',
    `peak_records`     int(11)               DEFAULT '1000' COMMENT 'Access peak per second, unit: records per second',
    `max_length`       int(11)               DEFAULT '10240' COMMENT 'The maximum length of a single piece of data, unit: Byte',
    `storage_period`   int(11)               DEFAULT '1' COMMENT 'The storage period of data in MQ, unit: day',
    `ext_params`       text                  DEFAULT NULL COMMENT 'Extended params, will be saved as JSON string',
    `status`           int(4)                DEFAULT '100' COMMENT 'Inlong stream status',
    `previous_status`  int(4)                DEFAULT '100' COMMENT 'Previous status',
    `is_deleted`       int(11)               DEFAULT '0' COMMENT 'Whether to delete, 0: not deleted, > 0: deleted',
    `creator`          varchar(64)  NOT NULL COMMENT 'Creator name',
    `modifier`         varchar(64)           DEFAULT NULL COMMENT 'Modifier name',
    `create_time`      timestamp    NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Create time',
    `modify_time`      timestamp    NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT 'Modify time',
    `version`          int(11)      NOT NULL DEFAULT '1' COMMENT 'Version number, which will be incremented by 1 after modification',
    PRIMARY KEY (`id`),
    UNIQUE KEY `unique_inlong_stream` (`inlong_stream_id`, `inlong_group_id`, `is_deleted`)
) ENGINE = InnoDB
  DEFAULT CHARSET = utf8mb4 COMMENT ='Inlong stream table';

-- ----------------------------
-- Table structure for inlong_stream_ext
-- ----------------------------
CREATE TABLE IF NOT EXISTS `inlong_stream_ext`
(
    `id`               int(11)      NOT NULL AUTO_INCREMENT COMMENT 'Incremental primary key',
    `inlong_group_id`  varchar(256) NOT NULL COMMENT 'Inlong group id',
    `inlong_stream_id` varchar(256) NOT NULL COMMENT 'Inlong stream id',
    `key_name`         varchar(256) NOT NULL COMMENT 'Configuration item name',
    `key_value`        text COMMENT 'The value of the configuration item',
    `is_deleted`       int(11)           DEFAULT '0' COMMENT 'Whether to delete, 0: not deleted, > 0: deleted',
    `modify_time`      timestamp    NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT 'Modify time',
    PRIMARY KEY (`id`),
    UNIQUE KEY `unique_inlong_stream_key` (`inlong_group_id`, `inlong_stream_id`, `key_name`),
    KEY `stream_id_index` (`inlong_group_id`, `inlong_stream_id`)
) ENGINE = InnoDB
  DEFAULT CHARSET = utf8mb4 COMMENT ='Inlong stream extension table';

-- ----------------------------
-- Table structure for inlong_stream_field
-- ----------------------------
CREATE TABLE IF NOT EXISTS `inlong_stream_field`
(
    `id`                  int(11)      NOT NULL AUTO_INCREMENT COMMENT 'Incremental primary key',
    `inlong_group_id`     varchar(256) NOT NULL COMMENT 'Owning inlong group id',
    `inlong_stream_id`    varchar(256) NOT NULL COMMENT 'Owning inlong stream id',
    `is_predefined_field` tinyint(1)   DEFAULT '0' COMMENT 'Whether it is a predefined field, 0: no, 1: yes',
    `field_name`          varchar(120) NOT NULL COMMENT 'field name',
    `field_value`         varchar(128) DEFAULT NULL COMMENT 'Field value, required if it is a predefined field',
    `pre_expression`      varchar(256) DEFAULT NULL COMMENT 'Pre-defined field value expression',
    `field_type`          varchar(20)  NOT NULL COMMENT 'field type',
    `field_comment`       varchar(50)  DEFAULT NULL COMMENT 'Field description',
    `is_meta_field`       smallint(3)  DEFAULT '0' COMMENT 'Is this field a meta field? 0: no, 1: yes',
    `meta_field_name`     varchar(20)  DEFAULT NULL COMMENT 'Meta field name',
    `field_format`        varchar(50)  DEFAULT NULL COMMENT 'Field format, including: MICROSECONDS, MILLISECONDS, SECONDS, SQL, ISO_8601 and custom such as yyyy-MM-dd HH:mm:ss',
    `rank_num`            smallint(6)  DEFAULT '0' COMMENT 'Field order (front-end display field order)',
    `is_deleted`          int(11)      DEFAULT '0' COMMENT 'Whether to delete, 0: not deleted, > 0: deleted',
    PRIMARY KEY (`id`),
    KEY `field_stream_id_index` (`inlong_stream_id`)
) ENGINE = InnoDB
  DEFAULT CHARSET = utf8mb4 COMMENT ='File/DB data source field table';

-- ----------------------------
-- Table structure for operation_log
-- ----------------------------
CREATE TABLE IF NOT EXISTS `operation_log`
(
    `id`                  int(11)   NOT NULL AUTO_INCREMENT,
    `authentication_type` varchar(64)        DEFAULT NULL COMMENT 'Authentication type',
    `operation_type`      varchar(256)       DEFAULT NULL COMMENT 'operation type',
    `http_method`         varchar(64)        DEFAULT NULL COMMENT 'Request method',
    `invoke_method`       varchar(256)       DEFAULT NULL COMMENT 'invoke method',
    `operator`            varchar(256)       DEFAULT NULL COMMENT 'operator',
    `proxy`               varchar(256)       DEFAULT NULL COMMENT 'proxy',
    `request_url`         varchar(256)       DEFAULT NULL COMMENT 'Request URL',
    `remote_address`      varchar(256)       DEFAULT NULL COMMENT 'Request IP',
    `cost_time`           bigint(20)         DEFAULT NULL COMMENT 'time-consuming',
    `body`                text COMMENT 'Request body',
    `param`               text COMMENT 'parameter',
    `status`              int(4)             DEFAULT NULL COMMENT 'status',
    `request_time`        timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT 'request time',
    `err_msg`             text COMMENT 'Error message',
    PRIMARY KEY (`id`)
) ENGINE = InnoDB
  DEFAULT CHARSET = utf8mb4;

-- ----------------------------
-- Table structure for source_file_basic
-- ----------------------------
CREATE TABLE IF NOT EXISTS `source_file_basic`
(
    `id`                int(11)      NOT NULL AUTO_INCREMENT COMMENT 'ID',
    `inlong_group_id`   varchar(256) NOT NULL COMMENT 'Inlong group id',
    `inlong_stream_id`  varchar(256) NOT NULL COMMENT 'Inlong stream id',
    `is_hybrid_source`  tinyint(1)        DEFAULT '0' COMMENT 'Whether to mix data sources',
    `is_table_mapping`  tinyint(1)        DEFAULT '0' COMMENT 'Is there a table name mapping',
    `date_offset`       int(4)            DEFAULT '0' COMMENT 'Time offset\n',
    `date_offset_unit`  varchar(2)        DEFAULT 'H' COMMENT 'Time offset unit',
    `file_rolling_type` varchar(2)        DEFAULT 'H' COMMENT 'File rolling type',
    `upload_max_size`   int(4)            DEFAULT '120' COMMENT 'Upload maximum size',
    `need_compress`     tinyint(1)        DEFAULT '0' COMMENT 'Whether need compress',
    `is_deleted`        int(11)           DEFAULT '0' COMMENT 'Whether to delete, 0: not deleted, > 0: deleted',
    `creator`           varchar(64)  NOT NULL COMMENT 'Creator',
    `modifier`          varchar(64)       DEFAULT NULL COMMENT 'Modifier',
    `create_time`       timestamp    NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Create time',
    `modify_time`       timestamp    NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT 'Modify time',
    `temp_view`         text              DEFAULT NULL COMMENT 'temp view',
    PRIMARY KEY (`id`)
) ENGINE = InnoDB
  DEFAULT CHARSET = utf8mb4 COMMENT ='basic configuration of file data source';

-- ----------------------------
-- Table structure for source_file_detail
-- ----------------------------
CREATE TABLE IF NOT EXISTS `source_file_detail`
(
    `id`               int(11)      NOT NULL AUTO_INCREMENT COMMENT 'Incremental primary key',
    `inlong_group_id`  varchar(256) NOT NULL COMMENT 'Owning inlong group id',
    `inlong_stream_id` varchar(256) NOT NULL COMMENT 'Owning inlong stream id',
    `access_type`      varchar(20)       DEFAULT 'Agent' COMMENT 'Collection type, there are Agent, DataProxy client, LoadProxy, the file can only be Agent temporarily',
    `server_name`      varchar(64)       DEFAULT NULL COMMENT 'The name of the data source service. If it is empty, add configuration through the following fields',
    `ip`               varchar(128) NOT NULL COMMENT 'Data source IP address',
    `port`             int(11)      NOT NULL COMMENT 'Data source port number',
    `is_inner_ip`      tinyint(1)        DEFAULT '0' COMMENT 'Whether it is intranet, 0: no, 1: yes',
    `issue_type`       varchar(10)       DEFAULT 'SSH' COMMENT 'Issuing method, there are SSH, TCS',
    `username`         varchar(32)       DEFAULT NULL COMMENT 'User name of the data source IP host',
    `password`         varchar(64)       DEFAULT NULL COMMENT 'The password corresponding to the above user name',
    `file_path`        varchar(256) NOT NULL COMMENT 'File path, supports regular matching',
    `status`           int(4)            DEFAULT '0' COMMENT 'Data source status',
    `previous_status`  int(4)            DEFAULT '0' COMMENT 'Previous status',
    `is_deleted`       int(11)           DEFAULT '0' COMMENT 'Whether to delete, 0: not deleted, > 0: deleted',
    `creator`          varchar(64)  NOT NULL COMMENT 'Creator name',
    `modifier`         varchar(64)       DEFAULT NULL COMMENT 'Modifier name',
    `create_time`      timestamp    NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Create time',
    `modify_time`      timestamp    NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT 'Modify time',
    `temp_view`        text              DEFAULT NULL COMMENT 'Temporary view, used to save un-submitted and unapproved intermediate data after modification',
    PRIMARY KEY (`id`)
) ENGINE = InnoDB
  DEFAULT CHARSET = utf8mb4 COMMENT ='Detailed table of file data source';

-- ----------------------------
-- Table structure for stream_source
-- ----------------------------
CREATE TABLE IF NOT EXISTS `stream_source`
(
    `id`                  int(11)      NOT NULL AUTO_INCREMENT COMMENT 'ID',
    `inlong_group_id`     varchar(256) NOT NULL COMMENT 'Inlong group id',
    `inlong_stream_id`    varchar(256) NOT NULL COMMENT 'Inlong stream id',
    `source_name`         varchar(128) NOT NULL DEFAULT '' COMMENT 'source_name',
    `source_type`         varchar(20)           DEFAULT '0' COMMENT 'Source type, including: FILE, DB, etc',
    `template_id`         int(11)               DEFAULT NULL COMMENT 'Id of the template task this agent belongs to',
    `agent_ip`            varchar(40)           DEFAULT NULL COMMENT 'Ip of the agent running the task, NULL if this is a template task',
    `uuid`                varchar(30)           DEFAULT NULL COMMENT 'Mac uuid of the agent running the task',
    `data_node_name`      varchar(128)          DEFAULT NULL COMMENT 'Node name, which links to data_node table',
    `inlong_cluster_name` varchar(128)          DEFAULT NULL COMMENT 'Cluster name of the agent running the task',
    `serialization_type`  varchar(20)           DEFAULT NULL COMMENT 'Serialization type, support: csv, json, canal, avro, etc',
    `snapshot`            text                  DEFAULT NULL COMMENT 'Snapshot of this source task',
    `report_time`         timestamp    NULL COMMENT 'Snapshot time',
    `ext_params`          text                  DEFAULT NULL COMMENT 'Another fields will be saved as JSON string, such as filePath, dbName, tableName, etc',
    `version`             int(11)               DEFAULT '1' COMMENT 'Stream source version',
    `status`              int(4)                DEFAULT '110' COMMENT 'Data source status',
    `previous_status`     int(4)                DEFAULT '110' COMMENT 'Previous status',
    `is_deleted`          int(11)               DEFAULT '0' COMMENT 'Whether to delete, 0: not deleted, > 0: deleted',
    `creator`             varchar(64)  NOT NULL COMMENT 'Creator name',
    `modifier`            varchar(64)           DEFAULT NULL COMMENT 'Modifier name',
    `create_time`         timestamp    NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Create time',
    `modify_time`         timestamp    NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT 'Modify time',
    PRIMARY KEY (`id`),
    UNIQUE KEY `unique_source_name` (`inlong_group_id`, `inlong_stream_id`, `source_name`, `is_deleted`),
    KEY `source_status_index` (`status`, `is_deleted`),
    KEY `source_agent_ip_index` (`agent_ip`, `is_deleted`),
    KEY `template_id_index` (`template_id`)
) ENGINE = InnoDB
  DEFAULT CHARSET = utf8mb4 COMMENT ='Stream source table';

-- ----------------------------
-- Table structure for stream_transform
-- ----------------------------
CREATE TABLE IF NOT EXISTS `stream_transform`
(
    `id`                   int(11)      NOT NULL AUTO_INCREMENT COMMENT 'Incremental primary key',
    `inlong_group_id`      varchar(256) NOT NULL COMMENT 'Inlong group id',
    `inlong_stream_id`     varchar(256) NOT NULL COMMENT 'Inlong stream id',
    `transform_name`       varchar(128) NOT NULL COMMENT 'Transform name, unique in one stream',
    `transform_type`       varchar(20)  NOT NULL COMMENT 'Transform type, including: splitter, filter, joiner, etc.',
    `pre_node_names`       text         NOT NULL COMMENT 'Pre node names of transform in this stream',
    `post_node_names`      text COMMENT 'Post node names of transform in this stream',
    `transform_definition` text         NOT NULL COMMENT 'Transform definition in json type',
    `version`              int(11)      NOT NULL DEFAULT '1' COMMENT 'Stream transform version',
    `is_deleted`           int(11)      NOT NULL DEFAULT '0' COMMENT 'Whether to delete, 0: not deleted, > 0: deleted',
    `creator`              varchar(64)  NOT NULL COMMENT 'Creator name',
    `modifier`             varchar(64)           DEFAULT '' COMMENT 'Modifier name',
    `create_time`          timestamp    NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Create time',
    `modify_time`          timestamp    NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT 'Modify time',
    PRIMARY KEY (`id`),
    UNIQUE KEY `unique_transform_name` (`inlong_group_id`, `inlong_stream_id`, `transform_name`, `is_deleted`)
) ENGINE = InnoDB
  DEFAULT CHARSET = utf8mb4 COMMENT ='Stream transform table';

-- ----------------------------
-- Table structure for stream_sink
-- ----------------------------
CREATE TABLE IF NOT EXISTS `stream_sink`
(
    `id`                     int(11)      NOT NULL AUTO_INCREMENT COMMENT 'Incremental primary key',
    `inlong_group_id`        varchar(256) NOT NULL COMMENT 'Owning inlong group id',
    `inlong_stream_id`       varchar(256) NOT NULL COMMENT 'Owning inlong stream id',
    `sink_type`              varchar(15)           DEFAULT 'HIVE' COMMENT 'Sink type, including: HIVE, ES, etc',
    `sink_name`              varchar(128) NOT NULL DEFAULT '' COMMENT 'Sink name',
    `description`            varchar(500) NULL COMMENT 'Sink description',
    `enable_create_resource` tinyint(1)            DEFAULT '1' COMMENT 'Whether to enable create sink resource? 0-disable, 1-enable',
    `inlong_cluster_name`    varchar(128)          DEFAULT NULL COMMENT 'Cluster name, which links to inlong_cluster table',
    `data_node_name`         varchar(128)          DEFAULT NULL COMMENT 'Node name, which links to data_node table',
    `sort_task_name`         varchar(512)          DEFAULT NULL COMMENT 'Sort task name or task ID',
    `sort_consumer_group`    varchar(512)          DEFAULT NULL COMMENT 'Consumer group name for Sort task',
    `ext_params`             text         NULL COMMENT 'Another fields, will be saved as JSON type',
    `operate_log`            text                  DEFAULT NULL COMMENT 'Background operate log',
    `status`                 int(11)               DEFAULT '100' COMMENT 'Status',
    `previous_status`        int(11)               DEFAULT '100' COMMENT 'Previous status',
    `is_deleted`             int(11)               DEFAULT '0' COMMENT 'Whether to delete, 0: not deleted, > 0: deleted',
    `creator`                varchar(64)  NOT NULL COMMENT 'Creator name',
    `modifier`               varchar(64)           DEFAULT NULL COMMENT 'Modifier name',
    `create_time`            timestamp    NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Create time',
    `modify_time`            timestamp    NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT 'Modify time',
    `version`                int(11)      NOT NULL DEFAULT '1' COMMENT 'Version number, which will be incremented by 1 after modification',
    PRIMARY KEY (`id`),
    UNIQUE KEY `unique_sink_name` (`inlong_group_id`, `inlong_stream_id`, `sink_name`, `is_deleted`)
) ENGINE = InnoDB
  DEFAULT CHARSET = utf8mb4 COMMENT ='Stream sink table';

-- ----------------------------
-- Table structure for stream_source_field
-- ----------------------------
CREATE TABLE IF NOT EXISTS `stream_source_field`
(
    `id`               int(11)      NOT NULL AUTO_INCREMENT COMMENT 'Incremental primary key',
    `inlong_group_id`  varchar(256) NOT NULL COMMENT 'Inlong group id',
    `inlong_stream_id` varchar(256) NOT NULL COMMENT 'Inlong stream id',
    `source_id`        int(11)      NOT NULL COMMENT 'Sink id',
    `source_type`      varchar(15)  NOT NULL COMMENT 'Sink type',
    `field_name`       varchar(120) NOT NULL COMMENT 'field name',
    `field_value`      varchar(128) DEFAULT NULL COMMENT 'Field value, required if it is a predefined field',
    `pre_expression`   varchar(256) DEFAULT NULL COMMENT 'Pre-defined field value expression',
    `field_type`       varchar(20)  NOT NULL COMMENT 'field type',
    `field_comment`    varchar(50)  DEFAULT NULL COMMENT 'Field description',
    `is_meta_field`    smallint(3)  DEFAULT '0' COMMENT 'Is this field a meta field? 0: no, 1: yes',
    `meta_field_name`  varchar(20)  DEFAULT NULL COMMENT 'Meta field name',
    `field_format`     varchar(50)  DEFAULT NULL COMMENT 'Field format, including: MICROSECONDS, MILLISECONDS, SECONDS, SQL, ISO_8601 and custom such as yyyy-MM-dd HH:mm:ss',
    `rank_num`         smallint(6)  DEFAULT '0' COMMENT 'Field order (front-end display field order)',
    `is_deleted`       int(11)      DEFAULT '0' COMMENT 'Whether to delete, 0: not deleted, > 0: deleted',
    PRIMARY KEY (`id`),
    KEY `source_id_index` (`source_id`)
) ENGINE = InnoDB
  DEFAULT CHARSET = utf8mb4 COMMENT ='Stream source field table';

-- ----------------------------
-- Table structure for stream_transform_field
-- ----------------------------
CREATE TABLE IF NOT EXISTS `stream_transform_field`
(
    `id`                int(11)      NOT NULL AUTO_INCREMENT COMMENT 'Incremental primary key',
    `inlong_group_id`   varchar(256) NOT NULL COMMENT 'Inlong group id',
    `inlong_stream_id`  varchar(256) NOT NULL COMMENT 'Inlong stream id',
    `transform_id`      int(11)      NOT NULL COMMENT 'Transform id',
    `transform_type`    varchar(15)  NOT NULL COMMENT 'Transform type',
    `field_name`        varchar(120) NOT NULL COMMENT 'Field name',
    `field_value`       varchar(128)  DEFAULT NULL COMMENT 'Field value, required if it is a predefined field',
    `pre_expression`    varchar(256)  DEFAULT NULL COMMENT 'Pre-defined field value expression',
    `field_type`        varchar(50)  NOT NULL COMMENT 'Field type',
    `field_comment`     varchar(2000) DEFAULT NULL COMMENT 'Field description',
    `is_meta_field`     smallint(3)   DEFAULT '0' COMMENT 'Is this field a meta field? 0: no, 1: yes',
    `meta_field_name`   varchar(20)   DEFAULT NULL COMMENT 'Meta field name',
    `field_format`      varchar(50)   DEFAULT NULL COMMENT 'Field format, including: MICROSECONDS, MILLISECONDS, SECONDS, SQL, ISO_8601 and custom such as yyyy-MM-dd HH:mm:ss',
    `rank_num`          smallint(6)   DEFAULT '0' COMMENT 'Field order (front-end display field order)',
    `is_deleted`        int(11)       DEFAULT '0' COMMENT 'Whether to delete, 0: not deleted, > 0: deleted',
    `origin_node_name`  varchar(256)  DEFAULT '' COMMENT 'Origin node name which stream field belongs',
    -- The source node name of the transport field
    `origin_field_name` varchar(50)   DEFAULT '' COMMENT 'Origin field name before transform operation',
    PRIMARY KEY (`id`),
    KEY `transform_id_index` (`transform_id`)
) ENGINE = InnoDB
  DEFAULT CHARSET = utf8mb4 COMMENT ='Stream transform field table';

-- ----------------------------
-- Table structure for stream_sink_field
-- ----------------------------
CREATE TABLE IF NOT EXISTS `stream_sink_field`
(
    `id`                int(11)      NOT NULL AUTO_INCREMENT COMMENT 'Incremental primary key',
    `inlong_group_id`   varchar(256) NOT NULL COMMENT 'Inlong group id',
    `inlong_stream_id`  varchar(256) NOT NULL COMMENT 'Inlong stream id',
    `sink_id`           int(11)      NOT NULL COMMENT 'Sink id',
    `sink_type`         varchar(15)  NOT NULL COMMENT 'Sink type',
    `source_field_name` varchar(50)   DEFAULT NULL COMMENT 'Source field name',
    `source_field_type` varchar(50)   DEFAULT NULL COMMENT 'Source field type',
    `field_name`        varchar(120) NOT NULL COMMENT 'Field name',
    `field_type`        varchar(50)  NOT NULL COMMENT 'Field type',
    `field_comment`     varchar(2000) DEFAULT NULL COMMENT 'Field description',
    `ext_params`        text COMMENT 'Field ext params',
    `is_meta_field`     smallint(3)   DEFAULT '0' COMMENT 'Is this field a meta field? 0: no, 1: yes',
    `meta_field_name`   varchar(20)   DEFAULT NULL COMMENT 'Meta field name',
    `field_format`      varchar(50)   DEFAULT NULL COMMENT 'Field format, including: MICROSECONDS, MILLISECONDS, SECONDS, SQL, ISO_8601 and custom such as yyyy-MM-dd HH:mm:ss',
    `origin_node_name`  varchar(256)  DEFAULT '' COMMENT 'Origin node name which stream field belongs',
    `origin_field_name` varchar(50)   DEFAULT '' COMMENT 'Origin field name before transform operation',
    `rank_num`          smallint(6)   DEFAULT '0' COMMENT 'Field order (front-end display field order)',
    `is_deleted`        int(11)       DEFAULT '0' COMMENT 'Whether to delete, 0: not deleted, > 0: deleted',
    PRIMARY KEY (`id`)
) ENGINE = InnoDB
  DEFAULT CHARSET = utf8mb4 COMMENT ='Stream sink field table';

-- ----------------------------
-- Table structure for user
-- ----------------------------
CREATE TABLE IF NOT EXISTS `user`
(
    `id`              int(11)      NOT NULL AUTO_INCREMENT COMMENT 'Incremental primary key',
    `name`            varchar(256) NOT NULL COMMENT 'Username',
    `password`        varchar(64)  NOT NULL COMMENT 'Password md5',
    `secret_key`      varchar(256)          DEFAULT NULL COMMENT 'Auth key for public network access',
    `public_key`      text                  DEFAULT NULL COMMENT 'Public key for asymmetric data encryption',
    `private_key`     text                  DEFAULT NULL COMMENT 'Private key for asymmetric data encryption',
    `encrypt_version` int(11)               DEFAULT NULL COMMENT 'Encryption key version',
    `account_type`    int(11)      NOT NULL DEFAULT '1' COMMENT 'Account type, 0-manager 1-normal',
    `due_date`        datetime              DEFAULT NULL COMMENT 'Due date for user',
    `ext_params`      text COMMENT 'Json extension info',
    `status`          int(11)               DEFAULT '100' COMMENT 'Status',
    `is_deleted`      int(11)               DEFAULT '0' COMMENT 'Whether to delete, 0 is not deleted, if greater than 0, delete',
    `creator`         varchar(256) NOT NULL COMMENT 'Creator name',
    `modifier`        varchar(256)          DEFAULT NULL COMMENT 'Modifier name',
    `create_time`     datetime     NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Create time',
    `modify_time`     datetime     NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT 'Modify time',
    `version`         int(11)      NOT NULL DEFAULT '1' COMMENT 'Version number, which will be incremented by 1 after modification',
    PRIMARY KEY (`id`),
    UNIQUE KEY `unique_user_name` (`name`)
) ENGINE = InnoDB
  DEFAULT CHARSET = utf8mb4 COMMENT ='User table';

-- create default admin user, username is 'admin', password is 'inlong'
INSERT INTO `user` (name, password,
                    secret_key, account_type, encrypt_version,
                    due_date, creator, modifier)
VALUES ('admin', '1976e096b31cfda81269d0df2775466aac6dd809e3ada1d5ba7831d85e80f109',
        '9B5DCE950F284141D5493A2DAFEBD1BFEECE075FC5F426E8B67F33F14876E2D0', 0, 1,
        '2099-12-31 23:59:59', 'inlong_init', 'inlong_init');

-- ----------------------------
-- Table structure for role
-- ----------------------------
CREATE TABLE IF NOT EXISTS `role`
(
    `id`          int(11)      NOT NULL AUTO_INCREMENT,
    `role_code`   varchar(100) NOT NULL COMMENT 'Role code',
    `role_name`   varchar(256) NOT NULL COMMENT 'Role Chinese name',
    `disabled`    tinyint(1)   NOT NULL DEFAULT '0' COMMENT 'Whether to disabled, 0: enabled, 1: disabled',
    `is_deleted`  int(11)               DEFAULT '0' COMMENT 'Whether to delete, 0 is not deleted, if greater than 0, delete',
    `creator`     varchar(256) NOT NULL COMMENT 'Creator name',
    `modifier`    varchar(256)          DEFAULT NULL COMMENT 'Modifier name',
    `create_time` datetime     NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Create time',
    `modify_time` datetime     NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT 'Modify time',
    `version`     int(11)      NOT NULL DEFAULT '1' COMMENT 'Version number, which will be incremented by 1 after modification',
    PRIMARY KEY (`id`),
    UNIQUE KEY `unique_role_code` (`role_code`),
    UNIQUE KEY `unique_role_name` (`role_name`)
) ENGINE = InnoDB
  DEFAULT CHARSET = utf8mb4 COMMENT ='Role Table';

-- ----------------------------
-- Table structure for user_role
-- ----------------------------
CREATE TABLE IF NOT EXISTS `user_role`
(
    `id`          int(11)      NOT NULL AUTO_INCREMENT,
    `user_name`   varchar(256) NOT NULL COMMENT 'Username',
    `role_code`   varchar(256) NOT NULL COMMENT 'User role code',
    `disabled`    tinyint(1)   NOT NULL DEFAULT '0' COMMENT 'Whether to disabled, 0: enabled, 1: disabled',
    `is_deleted`  int(11)               DEFAULT '0' COMMENT 'Whether to delete, 0 is not deleted, if greater than 0, delete',
    `creator`     varchar(256) NOT NULL COMMENT 'Creator name',
    `modifier`    varchar(256)          DEFAULT NULL COMMENT 'Modifier name',
    `create_time` datetime     NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Create time',
    `modify_time` datetime     NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT 'Modify time',
    `version`     int(11)      NOT NULL DEFAULT '1' COMMENT 'Version number, which will be incremented by 1 after modification',
    PRIMARY KEY (`id`)
) ENGINE = InnoDB
  DEFAULT CHARSET = utf8mb4 COMMENT ='User Role Table';

-- ----------------------------
-- Table structure for workflow_approver
-- ----------------------------
CREATE TABLE IF NOT EXISTS `workflow_approver`
(
    `id`           int(11)       NOT NULL AUTO_INCREMENT,
    `process_name` varchar(256)  NOT NULL COMMENT 'Process name',
    `task_name`    varchar(256)  NOT NULL COMMENT 'Approval task name',
    `approvers`    varchar(1024) NOT NULL COMMENT 'Approvers, separated by commas',
    `creator`      varchar(64)   NOT NULL COMMENT 'Creator name',
    `modifier`     varchar(64)            DEFAULT NULL COMMENT 'Modifier name',
    `create_time`  timestamp     NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Create time',
    `modify_time`  timestamp     NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT 'Modify time',
    `is_deleted`   int(11)                DEFAULT '0' COMMENT 'Whether to delete, 0 is not deleted, if greater than 0, delete',
    `version`      int(11)       NOT NULL DEFAULT '1' COMMENT 'Version number, which will be incremented by 1 after modification',
    PRIMARY KEY (`id`),
    KEY `process_name_task_name_index` (`process_name`, `task_name`)
) ENGINE = InnoDB
  DEFAULT CHARSET = utf8mb4 COMMENT ='Workflow approver table';

-- create default approver for new consumption and new inlong group
INSERT INTO `workflow_approver`(`process_name`, `task_name`, `approvers`, `creator`, `modifier`)
VALUES ('APPLY_CONSUMPTION_PROCESS', 'ut_admin', 'admin', 'inlong_init', 'inlong_init'),
       ('APPLY_GROUP_PROCESS', 'ut_admin', 'admin', 'inlong_init', 'inlong_init');

-- ----------------------------
-- Table structure for workflow_event_log
-- ----------------------------
CREATE TABLE IF NOT EXISTS `workflow_event_log`
(
    `id`                   int(11)      NOT NULL AUTO_INCREMENT,
    `process_id`           int(11)      NOT NULL,
    `process_name`         varchar(256)  DEFAULT NULL COMMENT 'Process name',
    `process_display_name` varchar(256) NOT NULL COMMENT 'Process name',
    `inlong_group_id`      varchar(256)  DEFAULT NULL COMMENT 'Inlong group id',
    `task_id`              int(11)       DEFAULT NULL COMMENT 'Task ID',
    `element_name`         varchar(256) NOT NULL COMMENT 'Name of the component that triggered the event',
    `element_display_name` varchar(256) NOT NULL COMMENT 'Display name of the component that triggered the event',
    `event_type`           varchar(64)  NOT NULL COMMENT 'Event type: process / task ',
    `event`                varchar(64)  NOT NULL COMMENT 'Event name',
    `listener`             varchar(1024) DEFAULT NULL COMMENT 'Event listener name',
    `status`               int(11)      NOT NULL COMMENT 'Status',
    `async`                tinyint(1)   NOT NULL COMMENT 'Asynchronous or not',
    `ip`                   varchar(64)   DEFAULT NULL COMMENT 'IP address executed by listener',
    `start_time`           datetime     NOT NULL COMMENT 'Monitor start execution time',
    `end_time`             datetime      DEFAULT NULL COMMENT 'Listener end time',
    `remark`               text COMMENT 'Execution result remark information',
    `exception`            text COMMENT 'Exception information',
    PRIMARY KEY (`id`),
    INDEX event_group_status_index (`inlong_group_id`, `status`)
) ENGINE = InnoDB
  DEFAULT CHARSET = utf8mb4 COMMENT ='Workflow event log table';

-- ----------------------------
-- Table structure for workflow_process
-- ----------------------------
CREATE TABLE IF NOT EXISTS `workflow_process`
(
    `id`              int(11)      NOT NULL AUTO_INCREMENT,
    `name`            varchar(256) NOT NULL COMMENT 'Process name',
    `display_name`    varchar(256) NOT NULL COMMENT 'Process display name',
    `type`            varchar(256)          DEFAULT NULL COMMENT 'Process classification',
    `title`           varchar(256)          DEFAULT NULL COMMENT 'Process title',
    `inlong_group_id` varchar(256)          DEFAULT NULL COMMENT 'Inlong group id: to facilitate related inlong group',
    `applicant`       varchar(256) NOT NULL COMMENT 'Applicant',
    `status`          varchar(64)  NOT NULL COMMENT 'Status',
    `form_data`       mediumtext COMMENT 'Form information',
    `start_time`      datetime     NOT NULL COMMENT 'Start time',
    `end_time`        datetime              DEFAULT NULL COMMENT 'End time',
    `ext_params`      text         NULL COMMENT 'Another fields, will be saved as JSON type',
    `hidden`          tinyint(1)   NOT NULL DEFAULT '0' COMMENT 'Whether to hidden, 0: not hidden, 1: hidden',
    PRIMARY KEY (`id`),
    INDEX process_group_status_index (`inlong_group_id`, `status`)
) ENGINE = InnoDB
  DEFAULT CHARSET = utf8mb4 COMMENT ='Workflow process table';

-- ----------------------------
-- Table structure for workflow_task
-- ----------------------------
CREATE TABLE IF NOT EXISTS `workflow_task`
(
    `id`                   int(11)       NOT NULL AUTO_INCREMENT,
    `type`                 varchar(64)   NOT NULL COMMENT 'Task type: UserTask / ServiceTask',
    `process_id`           int(11)       NOT NULL COMMENT 'Process ID',
    `process_name`         varchar(256)  NOT NULL COMMENT 'Process name',
    `process_display_name` varchar(256)  NOT NULL COMMENT 'Process name',
    `name`                 varchar(256)  NOT NULL COMMENT 'Task name',
    `display_name`         varchar(256)  NOT NULL COMMENT 'Task display name',
    `applicant`            varchar(64)   DEFAULT NULL COMMENT 'Applicant',
    `approvers`            varchar(1024) NOT NULL COMMENT 'Approvers',
    `status`               varchar(64)   NOT NULL COMMENT 'Status',
    `operator`             varchar(256)  DEFAULT NULL COMMENT 'Actual operator',
    `remark`               varchar(1024) DEFAULT NULL COMMENT 'Remark information',
    `form_data`            mediumtext COMMENT 'Form information submitted by the current task',
    `start_time`           datetime      NOT NULL COMMENT 'Start time',
    `end_time`             datetime      DEFAULT NULL COMMENT 'End time',
    `ext_params`           text COMMENT 'Extended information-json',
    PRIMARY KEY (`id`),
    INDEX process_status_index (`process_id`, `status`),
    INDEX process_name_index (`process_id`, `name`)
) ENGINE = InnoDB
  DEFAULT CHARSET = utf8mb4 COMMENT ='Workflow task table';

-- ----------------------------
-- Table structure for db_collector_detail_task
-- ----------------------------
CREATE TABLE IF NOT EXISTS `db_collector_detail_task`
(
    `id`            int(11)      NOT NULL AUTO_INCREMENT COMMENT 'Incremental primary key',
    `main_id`       varchar(128) NOT NULL COMMENT 'main task id',
    `type`          int(11)      NOT NULL COMMENT 'task type',
    `time_var`      varchar(64)  NOT NULL COMMENT 'time variable',
    `db_type`       int(11)      NOT NULL COMMENT 'db type',
    `ip`            varchar(64)  NOT NULL COMMENT 'db ip',
    `port`          int(11)      NOT NULL COMMENT 'db port',
    `db_name`       varchar(64)  NULL COMMENT 'db name',
    `user`          varchar(64)  NULL COMMENT 'user name',
    `password`      varchar(64)  NULL COMMENT 'password',
    `sql_statement` varchar(256) NULL COMMENT 'sql statement',
    `offset`        int(11)      NOT NULL COMMENT 'offset for the data source',
    `total_limit`   int(11)      NOT NULL COMMENT 'total limit in a task',
    `once_limit`    int(11)      NOT NULL COMMENT 'limit for one query',
    `time_limit`    int(11)      NOT NULL COMMENT 'time limit for task',
    `retry_times`   int(11)      NOT NULL COMMENT 'max retry times if task failes',
    `group_id`      varchar(64)  NULL COMMENT 'group id',
    `stream_id`     varchar(64)  NULL COMMENT 'stream id',
    `state`         int(11)      NOT NULL COMMENT 'task state',
    `create_time`   timestamp    NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'create time',
    `modify_time`   timestamp    NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT 'modify time',
    PRIMARY KEY (`id`)
) ENGINE = InnoDB
  DEFAULT CHARSET = utf8mb4 COMMENT ='db collector detail task table';

-- ----------------------------
-- Table structure for sort_source_config
-- ----------------------------
CREATE TABLE IF NOT EXISTS `sort_source_config`
(
    `id`           int(11)      NOT NULL AUTO_INCREMENT COMMENT 'Incremental primary key',
    `cluster_name` varchar(128) NOT NULL COMMENT 'Cluster name',
    `task_name`    varchar(128) NOT NULL COMMENT 'Task name',
    `zone_name`    varchar(128) NOT NULL COMMENT 'Cache zone name',
    `topic`        varchar(128) DEFAULT '' COMMENT 'Topic',
    `ext_params`   text         DEFAULT NULL COMMENT 'Another fields, will be saved as JSON type',
    PRIMARY KEY (`id`),
    KEY `sort_source_config_index` (`cluster_name`, `task_name`)
) ENGINE = InnoDB
  DEFAULT CHARSET = utf8mb4 COMMENT ='Sort source config table';

-- ----------------------------
-- Table structure for inlong component heartbeat
-- ----------------------------
CREATE TABLE IF NOT EXISTS `component_heartbeat`
(
    `id`               int(11)     NOT NULL AUTO_INCREMENT COMMENT 'Incremental primary key',
    `component`        varchar(64) NOT NULL DEFAULT '' COMMENT 'Component name, such as: Agent, Sort...',
    `instance`         varchar(64) NOT NULL DEFAULT '' COMMENT 'Component instance, can be ip, name...',
    `status_heartbeat` text                 DEFAULT NULL COMMENT 'Status heartbeat info',
    `metric_heartbeat` text                 DEFAULT NULL COMMENT 'Metric heartbeat info',
    `report_time`      bigint(20)  NOT NULL COMMENT 'Report time',
    `create_time`      timestamp   NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Create time',
    `modify_time`      timestamp   NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT 'Modify time',
    PRIMARY KEY (`id`),
    UNIQUE KEY `unique_component_heartbeat` (`component`, `instance`)
) ENGINE = InnoDB
  DEFAULT CHARSET = utf8 COMMENT ='Inlong component heartbeat';

-- ----------------------------
-- Table structure for inlong group heartbeat
-- ----------------------------
CREATE TABLE IF NOT EXISTS `group_heartbeat`
(
    `id`               int(11)      NOT NULL AUTO_INCREMENT COMMENT 'Incremental primary key',
    `component`        varchar(64)  NOT NULL DEFAULT '' COMMENT 'Component name, such as: Agent, Sort...',
    `instance`         varchar(64)  NOT NULL DEFAULT '' COMMENT 'Component instance, can be ip, name...',
    `inlong_group_id`  varchar(256) NOT NULL DEFAULT '' COMMENT 'Owning inlong group id',
    `status_heartbeat` text                  DEFAULT NULL COMMENT 'Status heartbeat info',
    `metric_heartbeat` text                  DEFAULT NULL COMMENT 'Metric heartbeat info',
    `report_time`      bigint(20)   NOT NULL COMMENT 'Report time',
    `create_time`      timestamp    NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Create time',
    `modify_time`      timestamp    NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT 'Modify time',
    PRIMARY KEY (`id`),
    UNIQUE KEY `unique_group_heartbeat` (`component`, `instance`, `inlong_group_id`)
) ENGINE = InnoDB
  DEFAULT CHARSET = utf8 COMMENT ='Inlong group heartbeat';

-- ----------------------------
-- Table structure for inlong stream heartbeat
-- ----------------------------
CREATE TABLE IF NOT EXISTS `stream_heartbeat`
(
    `id`               int(11)      NOT NULL AUTO_INCREMENT COMMENT 'Incremental primary key',
    `component`        varchar(64)  NOT NULL DEFAULT '' COMMENT 'Component name, such as: Agent, Sort...',
    `instance`         varchar(64)  NOT NULL DEFAULT '' COMMENT 'Component instance, can be ip, name...',
    `inlong_group_id`  varchar(256) NOT NULL DEFAULT '' COMMENT 'Owning inlong group id',
    `inlong_stream_id` varchar(256) NOT NULL DEFAULT '' COMMENT 'Owning inlong stream id',
    `status_heartbeat` text                  DEFAULT NULL COMMENT 'Status heartbeat info',
    `metric_heartbeat` text                  DEFAULT NULL COMMENT 'Metric heartbeat info',
    `report_time`      bigint(20)   NOT NULL COMMENT 'Report time',
    `create_time`      timestamp    NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Create time',
    `modify_time`      timestamp    NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT 'Modify time',
    PRIMARY KEY (`id`),
    UNIQUE KEY `unique_stream_heartbeat` (`component`, `instance`, `inlong_group_id`, `inlong_stream_id`)
) ENGINE = InnoDB
  DEFAULT CHARSET = utf8 COMMENT ='Inlong stream heartbeat';

-- ----------------------------

SET FOREIGN_KEY_CHECKS = 1;
