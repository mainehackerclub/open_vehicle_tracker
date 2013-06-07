<?php

$roads = json_decode( file_get_contents( 'https://raw.github.com/mainehackerclub/bangor/working/data/Roads/RoadCenterline.geojson' ), true );

$dbconn = pg_connect( "host=141.114.192.128 port=5432 dbname=matador user=will password=will" );

/*pg_query( $dbconn, 'DELETE FROM telemetries WHERE vehicle_id = 1' );*/

$truck = 1;
foreach( $roads[ 'features' ] as $road ) {
	$truck++;
	if( $truck > 5 )
		$truck = 1;
	
	foreach( $road[ 'geometry' ][ 'coordinates' ] as $c ) {
		$q = "INSERT INTO telemetries (vehicle_id, lat, lon, time) VALUES ( " . $truck . ", " . $c[ 1 ] . "," . $c[ 0 ] . ", '" . date( 'Y-m-d H:i:s' ) . "' )";
		echo $q . "\n";
		pg_query( $dbconn, $q ); 
		sleep( 3 );
	}

}
