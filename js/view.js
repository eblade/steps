(function (window) {
    'use strict';

    function View() {
        this.sequenceTemplate
        =   '<div seq-id="{{id}}" class="{{enabled}}">\n'
        +   '    <div class="info">{{title}}</div>\n'
        +   '    <div class="blocks">\n'
        +   '    </div>\n'
        +   '</div>\n';
    }

    View.prototype.show = function (data) {
        var
            i,
            view = '';
       
        console.log("SHOW " + data.length + " SEQUENCES");
        for (i = 0; i < data.length; i++) {
            var
                template = this.sequenceTemplate,
                enabled = 'disabled';

            if (data[i].enabled === true) {
                enabled = 'enabled';
            }

            console.log("SHOW SEQUENCE ", data[i].id);

            template = template.replace('{{id}}', data[i].id);
            template = template.replace('{{title}}', data[i].title || ('[' + data[i].id + ']'));
            template = template.replace('{{enabled}}', enabled);

            view += template;
        }

        return view;
    };

    window.app.View = View;

})(window);
