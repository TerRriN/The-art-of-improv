$('.initiator').on('mouseenter mouseleave', function(e) {
    $('.receiver').trigger(e.type);
})


//måste fixas till