/*
Navicat SQLite Data Transfer

Source Server         : Config
Source Server Version : 30706
Source Host           : :0

Target Server Type    : SQLite
Target Server Version : 30706
File Encoding         : 65001

Date: 2013-10-19 20:21:14
*/

PRAGMA foreign_keys = OFF;

-- ----------------------------
-- Table structure for "main"."charset"
-- ----------------------------
CREATE TABLE "charset" (
"id_"  integer PRIMARY KEY AUTOINCREMENT,
"name_"  varchar(255),
"type_"  INTEGER,
"desc_"  varchar(255),
UNIQUE ("name_" ASC)
);


-- ----------------------------
-- Records of charset
-- ----------------------------
INSERT INTO "main"."charset" VALUES (null, 'ASCII', 2, null);
INSERT INTO "main"."charset" VALUES (null, 'Big5', 0, null);
INSERT INTO "main"."charset" VALUES (null, 'EUC-JP', 0, null);
INSERT INTO "main"."charset" VALUES (null, 'EUC-KR', 0, null);
INSERT INTO "main"."charset" VALUES (null, 'GB18030', 2, null);
INSERT INTO "main"."charset" VALUES (null, 'HZ-GB-2312', 0, null);
INSERT INTO "main"."charset" VALUES (null, 'UTF-8', 2, null);
INSERT INTO "main"."charset" VALUES (null, 'UTF-16BE', 2, null);
INSERT INTO "main"."charset" VALUES (null, 'UTF-16LE', 2, null);
INSERT INTO "main"."charset" VALUES (null, 'UTF-32BE', 2, null);
INSERT INTO "main"."charset" VALUES (null, 'UTF-32LE', 2, null);
INSERT INTO "main"."charset" VALUES (null, 'EUC-CN', 1, null);
INSERT INTO "main"."charset" VALUES (null, 'HZ', 1, null);
INSERT INTO "main"."charset" VALUES (null, 'GBK', 1, null);
INSERT INTO "main"."charset" VALUES (null, 'EUC-TW', 1, null);
INSERT INTO "main"."charset" VALUES (null, 'BIG5', 1, null);
INSERT INTO "main"."charset" VALUES (null, 'BIG5-HKSCS', 1, null);
INSERT INTO "main"."charset" VALUES (null, 'BIG5-HKSCS:2004', 1, null);
INSERT INTO "main"."charset" VALUES (null, 'BIG5-HKSCS:2001', 1, null);
INSERT INTO "main"."charset" VALUES (null, 'BIG5-HKSCS:1999', 1, null);
INSERT INTO "main"."charset" VALUES (null, 'UCS-2', 1, null);
INSERT INTO "main"."charset" VALUES (null, 'UCS-2BE', 1, null);
INSERT INTO "main"."charset" VALUES (null, 'UCS-2LE', 1, null);
INSERT INTO "main"."charset" VALUES (null, 'UCS-4', 1, null);
INSERT INTO "main"."charset" VALUES (null, 'UCS-4BE', 1, null);
INSERT INTO "main"."charset" VALUES (null, 'UCS-4LE', 1, null);
INSERT INTO "main"."charset" VALUES (null, 'UTF-16', 1, null);
INSERT INTO "main"."charset" VALUES (null, 'UTF-32', 1, null);
INSERT INTO "main"."charset" VALUES (null, 'UTF-7', 1, null);
