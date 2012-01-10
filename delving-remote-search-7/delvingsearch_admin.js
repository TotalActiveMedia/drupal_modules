/**
 * @file delvingsearch_admin.js
 *
 * TODO: Write description
 */

(function($) {	
	$(document).ready(function() {
		var select = $('td:not(.tabledrag-hide)').find('select');
		$.each(select, function() {
			var val = $(this).val();
			if (val === 'hidden') {
				var row = $(this).parent().parent().parent();
				$(row).css("background-color", "#39424a");
			}
		});
		$(select).change(function () {
			var val = $(this).val();
			var row = $(this).parent().parent().parent();
			if (val === 'hidden') {
				$(row).css("background-color", "#39424a");
			}
			else {
				$(row).css("background-color", "");
			}
		});
	})
})(jQuery);