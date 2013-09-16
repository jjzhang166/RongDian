/*
Navicat SQLite Data Transfer

Source Server         : Config
Source Server Version : 30706
Source Host           : :0

Target Server Type    : SQLite
Target Server Version : 30706
File Encoding         : 65001

Date: 2013-09-15 02:10:27
*/

PRAGMA foreign_keys = OFF;

-- ----------------------------
-- Table structure for "main"."config"
-- ----------------------------
DROP TABLE "main"."config";
CREATE TABLE config(id_ integer primary key, name_ varchar(100) unique, value_ varchar(255));

-- ----------------------------
-- Records of config
-- ----------------------------
INSERT INTO "main"."config" VALUES (1, 'Background', 'ff32506e');
INSERT INTO "main"."config" VALUES (2, 'Lang', 'lang_en.ini');
