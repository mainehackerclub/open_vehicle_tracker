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
    <link href="main.css" rel="stylesheet" type="text/css">
    <script src="https://maps.googleapis.com/maps/api/js?v=3.exp&sensor=false&libraries=geometry"></script>
    <script src="main.js"></script>
    <script src="panel.js"></script>
    <link rel="stylesheet" type="text/css" href="bootstrap/css/bootstrap.min.css">
  </head>
  <body>
<!--    <div class="container-fluid"> -->
    <div class="row-fluid">
      <div class="span3">
        <div class="my_sidebar">
       <!-- <div class="well sidebar-nav"> -->
        <div class="vehicle_list">
            <?php 
            $dbconn = pg_connect( 'host=localhost port=5432 dbname=matador user=plow password=notverysecure' );
            $q = "SELECT type FROM vehicles GROUP BY type";
            $result = pg_fetch_all( pg_query( $dbconn, $q ) );
            foreach($result as $r){
              $collapsible_string = str_replace(' ', '_', $r['type']);
            ?>
              <a href="#" id=<?php echo $r['type'] ?> data-toggle="collapse" class="accordion-body collapse" data-target="#collapsible_<?php echo $collapsible_string; ?>"></i><?php echo $r['type'] ?></a>
              <div id="collapsible_<?php echo $collapsible_string ?>" class="collapse in">
              <ul class="nav nav-list">
              <?php
              $i=1;
              $q = "SELECT * FROM vehicles WHERE type='". $r['type'] ."' ORDER BY _id ASC;";
              $vehiclesResult = pg_fetch_all(pg_query($q));
              foreach($vehiclesResult as $vehicle){
                echo '<li onclick=\'vehiclePopup(this)\'>'. $r['type'] .' '. $i .'</a></li> ';
                $i++;
              }
              ?>
              </ul>
            </div>
          <?php } ?>
          </div>
          </div>
<!--        </div> -->
      </div>
      <div class="span9">
        <div id="map_canvas" ></div>
      </div>
    </div>
<!--    </div> -->
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
    <script src="bootstrap/js/bootstrap.min.js"></script>
  </body>
</html>
