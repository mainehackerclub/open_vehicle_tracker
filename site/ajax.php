<?php
header( 'Access-Control-Allow-Origin: *', true );
header( 'Content-Type: application/json; charset=UTF-8', true );
header( 'Expires: ' . gmdate( 'D, d M Y H:i:s', 0 ) . ' GMT', true );
header( 'Cache-Control: private, no-cache, max-age=0', true );
header( 'Pragma: private', true );

$dbconn = pg_connect( 'host=localhost port=5432 dbname=matador user=plow password=notverysecure' );

$q = 'SELECT * from telemetries WHERE 1=1';
if( !empty( $_GET[ 'lastid' ] ) )
  $q .= ' AND _id > ' . $_GET[ 'lastid' ];
if(isset($_GET['vehicleid']) && !empty($_GET['vehicleid']))
  $q .= " AND vehicle_id = '". pg_escape_string($_GET['vehicleid']) ."'";
$q .= " AND (lat != 0 AND lon != 0)";
if(isset($_GET['vehicleid']) && !empty($_GET['vehicleid'])){
  $q .= " ORDER BY date DESC, time DESC LIMIT 1";
} else {
  $q .= ' ORDER BY date ACS, time ASC';
}
$result = pg_fetch_all( pg_query( $dbconn, $q ) );

$array = array();
if( !empty( $result ) ) { 
	foreach( $result as $r ) {
		$array[] = array(
			'truckid' => $r[ 'vehicle_id' ],
			'lat' => $r[ 'lat' ],
			'lon' => $r[ 'lon' ],
			'lastid' => $r[ '_id' ],
		);
	}
} else {
  echo "AHH EMPTY DATASET";
}

//echo 'yo';
if( empty( $_GET[ 'jsonp' ] ) ) {
	echo json_encode( $array );
} else {
	echo $_GET[ 'jsonp' ] . '( ' . json_encode( $array ) . ' )';
}
