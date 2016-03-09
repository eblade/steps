(function (window) {
    'use strict';

    function Store(name, callback) {
        var
            data,
            dbName;

        callback = callback || function () {};

        dbName = this._dbName = name;

        chrome.storage.local.get(dbName, function (storage) {
            if (dbName in storage) {
                callback.call(this, storage[dbName].sequences);
            } else {
                storage = {};
                storage[dbName] = { sequences: [] };
                chrome.storage.local.set(storage, function () {
                    callback.call(this, storage[dbName].sequences);
                }.bind(this));
            }
        }.bind(this));
    };

    Store.prototype.loadAll = function (callback) {
        callback = callback || function () {};
        console.log("STORE ABOUT TO CALL STORE " + this._dbName);
        chrome.storage.local.get(this._dbName, function (storage) {
            var sequences = storage[this._dbName] && storage[this._dbName].sequences || [];
            console.log("GOT SEQUENCES FROM STORE " + sequences.length);
            callback.call(this, sequences);
        }.bind(this));
    };

    Store.prototype.save = function (updateData, callback) {
        console.log("STORE SAVE");
        chrome.storage.local.get(this._dbName, function (storage) {
            var
                id = updateData.id,
                data = storage[this._dbName],
                sequences = data.sequences;

            callback = callback || function () {};
            console.log('STORE ID = ' + id);
            
            if (id) {
                for (var i = 0; i < sequences.length; i++) {
                    if (sequences[i].id === id) {
                        for (var x in updateData) {
                            sequences[i][x] = updateData[x];
                        }
                    }
                }
                chrome.storage.local.set(storage, function (storage) {
                    chrome.storage.local.get(this._dbName, function (storage) {
                        callback.call(this, storage[this._dbName].sequences);
                    }.bind(this));
                }.bind(this));
            } else {
                updateData.id = (new Date()).getTime();
                console.log("STORE SEQUNCE AS " + updateData.id);
                sequences.push(updateData);
                chrome.storage.local.set(storage, function (storage) {
                    callback.call(this, [updateData]);
                }.bind(this));
            }


        }.bind(this));
    };

    window.app.Store = Store;
})(window);
