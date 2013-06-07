<?php error_reporting(E_ALL); 
ini_set('display_errors',"On");
?>
<!DOCTYPE html>
<html>
  <head>
    <title>Truck tracker</title>
    <meta name="viewport" content="initial-scale=1.0, user-scalable=no">
    <script src="http://ajax.googleapis.com/ajax/libs/jquery/1.10.0/jquery.min.js"></script>
    <script src="http://ajax.googleapis.com/ajax/libs/jqueryui/1.10.3/jquery-ui.min.js"></script>
    <meta charset="utf-8">
    <link rel="stylesheet" type="text/css" href="main.css">
    <script src="https://maps.googleapis.com/maps/api/js?v=3.exp&sensor=false&libraries=geometry"></script>
    <script src="main.js"></script>
    <script src="panel.js"></script>
  </head>
  <body>
    <div id="map-canvas"></div>
    <div id="info-panel-border"  class="glass">
      <div id="info-panel" class="glass">
        <dl class="accordion">
<?php 
$dbconn = pg_connect( 'host=localhost port=5432 dbname=matador user=plow password=notverysecure' );
$q = "SELECT type FROM vehicles GROUP BY type";
$result = pg_fetch_all( pg_query( $dbconn, $q ) );
foreach($result as $r){
?>
    
    <dt class="panel"><input onclick="checkEvent();" type='checkbox' class="<?php echo $r['type']; ?>" id="<?php echo $r['type'] . $i; ?>"><a href=""><?php echo $r['type']; ?></a></dt>
            <dd>
              <ul class = "panel-content">
<?php
  $i=1;
  $q = "SELECT * FROM vehicles WHERE type='". $r['type'] ."';";
  $vehiclesResult = pg_fetch_all(pg_query($q));
  foreach($vehiclesResult as $vehicle){
                echo '<li id='.$i.' onclick=\'vehiclePopup(this)\'> ' . $r['type'] .$i.'</li>';
               $i++;
  }
              ?>
            </ul>
          </dd>
<?php } ?>
        </dl>
      </div>
    </div>
  <script>
    function checkEvent(){
      if(document.getElementByClassName(this.class).checked){
        uncheck(this);
      } else {
        check(this);
      }
    }
    function check(item)
    {

    }
    function uncheck(item)
    {
      for(var i = 0; i<markers.length; i++) {
       //if(markers[i] = 
      console.log(markers);
      }
    }
  </script>
  </body>
</html>
