<?php
/*
 * cidrl
 *
 * Lists all IP addresses within a CIDR block.
 *
 * @author Damien Bezborodov
 * @link https://github.com/emden-norfolk/cidrl
 */
function cidrl($cidr, &$error_code = 0) {
	sscanf($cidr, "%[^/]/%u", $network, $bits);
	$addr = ip2long($network);
	if ($addr === false) {
		$error_code = 2;
		return false;
	}

	if ($bits == 32) {
		return array(long2ip($addr));
	}

	if ($bits > 32) {
		$error_code = 3;
		return false;
	}

	$mask = ~(0xFFFFFFFF >> $bits);

	$addr_start = $addr & $mask;
	$addr_end = ($addr & $mask) | ~$mask;

	$addresses = array();
	for ($i = $addr_start; $i <= $addr_end; $i++) {
		$addresses[] = long2ip($i);
	}
	return $addresses;
}
