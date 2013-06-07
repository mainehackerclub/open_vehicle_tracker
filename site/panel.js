$(document).ready(function(){
  var allPanels = $('.accordion > dd').hide();

  $('.accordion > dt > a').click(function() {
    allPanels.slideUp();
    
    if($(this).parent().next().is(':hidden'))
    {
        $(this).parent().next().slideDown();
    }
    
    return false;
  });
  jQuery('#info-panel').click(function(){
    jQuery("#info-panel-border").toggleClass("hidden");
  });
});
