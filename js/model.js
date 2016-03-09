(function (window) {
    'use strict';

    function Model(storage) {
        this.storage = storage;
    }

    Model.prototype.loadSequences = function (callback) {
        callback = callback || function () {};
        console.log('MODEL LOADING ' + callback);
        return this.storage.loadAll(callback);
    };

    Model.prototype.createSequence = function (title, callback) {
        title = title || '';
        callback = callback || function () {};

        var newSequence = {
            title: title.trim(),
            enabled: true,
            length: 8,
            resolution: [1, 8]
        };

        console.log('STORE SEQUENCE ' + newSequence);

        this.storage.save(newSequence, callback);
    };

    window.app.Model = Model;
})(window);
