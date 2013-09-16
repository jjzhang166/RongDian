/*
Navicat SQLite Data Transfer

Source Server         : Config
Source Server Version : 30706
Source Host           : :0

Target Server Type    : SQLite
Target Server Version : 30706
File Encoding         : 65001

Date: 2013-09-15 02:09:45
*/

PRAGMA foreign_keys = OFF;

-- ----------------------------
-- Table structure for "main"."addr"
-- ----------------------------
DROP TABLE "main"."addr";
CREATE TABLE addr(id_ integer primary key, solution_ varchar(100) unique, atype_ integer, addr_ varchar(255), mask_ varchar(100), gateway_ varchar(100), dtype_ integer, dns1_ varchar(100), dns2_ varchar(100), valid_ integer);

-- ----------------------------
-- Records of addr
-- ----------------------------
INSERT INTO "main"."addr" VALUES (1, 'IP Solution0', 0, null, null, null, 0, null, null, 1);
INSERT INTO "main"."addr" VALUES (2, 'IP Solution1', 1, '192.168.1.101', '255.255.255.0', '192.168.1.1', 1, '192.168.1.1', null, 1);
INSERT INTO "main"."addr" VALUES (3, 'IP Solution2', 1, '192.168.0.101', '255.255.128.0', '192.168.0.1', 1, '192.168.0.1', '192.168.0.2', 1);
