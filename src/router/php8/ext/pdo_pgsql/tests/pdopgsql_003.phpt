--TEST--
Pdo\Pgsql getWarningCount
--EXTENSIONS--
pdo
pdo_pgsql
--SKIPIF--
<?php
require __DIR__ . '/config.inc';
require dirname(__DIR__, 2) . '/pdo/tests/pdo_test.inc';
PDOTest::skip();
?>
--FILE--
<?php

require_once __DIR__ . "/config.inc";

$db =  Pdo::connect($config['ENV']['PDOTEST_DSN']);
if (!$db instanceof Pdo\Pgsql) {
    echo "Wrong class type. Should be Pdo\Pgsql but is " . get_class($db) . "\n";
}

echo $db->escapeIdentifier("This is a quote\"") . "\n";

try {
    $db->escapeIdentifier("aa\xC3\xC3\xC3");
} catch (PDOException $e) {
    echo $e->getMessage() . "\n";
}

--EXPECTF--
"This is a quote"""
SQLSTATE[HY000]: General error: 7 %r(incomplete|invalid)%r multibyte character
