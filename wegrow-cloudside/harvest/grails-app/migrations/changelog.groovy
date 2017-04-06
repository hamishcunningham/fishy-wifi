databaseChangeLog = {
    changeSet(author: "dominic (generated)", id: "1488892755677-1") {
        createTable(tableName: "area") {
            column(autoIncrement: "true", name: "id", type: "BIGINT") {
                constraints(primaryKey: "true", primaryKeyName: "areaPK")
            }

            column(name: "version", type: "BIGINT") {
                constraints(nullable: "false")
            }

            column(name: "area_meters", type: "DOUBLE")

            column(name: "canopy_radius_meters", type: "DOUBLE")

            column(name: "crop_id", type: "BIGINT") {
                constraints(nullable: "false")
            }

            column(name: "finished", type: "BOOLEAN")

            column(name: "in_greenhouse", type: "BOOLEAN")

            column(name: "name", type: "VARCHAR(255)")

            column(name: "space_id", type: "BIGINT") {
                constraints(nullable: "false")
            }

            column(name: "variety", type: "VARCHAR(255)")
        }
    }

    changeSet(author: "dominic (generated)", id: "1488892755677-2") {
        createTable(tableName: "crop") {
            column(autoIncrement: "true", name: "id", type: "BIGINT") {
                constraints(primaryKey: "true", primaryKeyName: "cropPK")
            }

            column(name: "version", type: "BIGINT") {
                constraints(nullable: "false")
            }

            column(name: "is_greenhouseable", type: "BOOLEAN") {
                constraints(nullable: "false")
            }

            column(name: "is_tree", type: "BOOLEAN") {
                constraints(nullable: "false")
            }

            column(name: "type", type: "VARCHAR(255)") {
                constraints(nullable: "false")
            }
        }
    }

    changeSet(author: "dominic (generated)", id: "1488892755677-3") {
        createTable(tableName: "growing_space") {
            column(autoIncrement: "true", name: "id", type: "BIGINT") {
                constraints(primaryKey: "true", primaryKeyName: "growing_spacePK")
            }

            column(name: "version", type: "BIGINT") {
                constraints(nullable: "false")
            }

            column(name: "area_meters", type: "DOUBLE") {
                constraints(nullable: "false")
            }

            column(name: "consent_given", type: "BOOLEAN") {
                constraints(nullable: "false")
            }

            column(name: "electronic_signature", type: "VARCHAR(255)") {
                constraints(nullable: "false")
            }

            column(name: "is_organic", type: "BOOLEAN") {
                constraints(nullable: "false")
            }

            column(name: "months_growing", type: "INT") {
                constraints(nullable: "false")
            }

            column(name: "submitting_all_data", type: "BOOLEAN") {
                constraints(nullable: "false")
            }

            column(name: "years_growing", type: "INT") {
                constraints(nullable: "false")
            }

            column(name: "class", type: "VARCHAR(255)") {
                constraints(nullable: "false")
            }

            column(name: "post_code", type: "VARCHAR(255)")

            column(name: "type", type: "VARCHAR(255)")

            column(name: "address", type: "VARCHAR(255)")

            column(name: "site_name", type: "VARCHAR(255)")

            column(name: "visits_per_week", type: "INT")
        }
    }

    changeSet(author: "dominic (generated)", id: "1488892755677-4") {
        createTable(tableName: "growing_space_harvest") {
            column(name: "growing_space_harvests_id", type: "BIGINT") {
                constraints(nullable: "false")
            }

            column(name: "harvest_id", type: "BIGINT")
        }
    }

    changeSet(author: "dominic (generated)", id: "1488892755677-5") {
        createTable(tableName: "harvest") {
            column(autoIncrement: "true", name: "id", type: "BIGINT") {
                constraints(primaryKey: "true", primaryKeyName: "harvestPK")
            }

            column(name: "version", type: "BIGINT") {
                constraints(nullable: "false")
            }

            column(name: "area_id", type: "BIGINT") {
                constraints(nullable: "false")
            }

            column(name: "date_created", type: "timestamp") {
                constraints(nullable: "false")
            }

            column(name: "weight_grammes", type: "DOUBLE") {
                constraints(nullable: "false")
            }
        }
    }

    changeSet(author: "dominic (generated)", id: "1488892755677-6") {
        createTable(tableName: "registration_code") {
            column(autoIncrement: "true", name: "id", type: "BIGINT") {
                constraints(primaryKey: "true", primaryKeyName: "registration_codePK")
            }

            column(name: "date_created", type: "timestamp") {
                constraints(nullable: "false")
            }

            column(name: "token", type: "VARCHAR(255)") {
                constraints(nullable: "false")
            }

            column(name: "username", type: "VARCHAR(255)") {
                constraints(nullable: "false")
            }
        }
    }

    changeSet(author: "dominic (generated)", id: "1488892755677-7") {
        createTable(tableName: "role") {
            column(autoIncrement: "true", name: "id", type: "BIGINT") {
                constraints(primaryKey: "true", primaryKeyName: "rolePK")
            }

            column(name: "version", type: "BIGINT") {
                constraints(nullable: "false")
            }

            column(name: "authority", type: "VARCHAR(255)") {
                constraints(nullable: "false")
            }
        }
    }

    changeSet(author: "dominic (generated)", id: "1488892755677-8") {
        createTable(tableName: "settings") {
            column(autoIncrement: "true", name: "id", type: "BIGINT") {
                constraints(primaryKey: "true", primaryKeyName: "settingsPK")
            }

            column(name: "version", type: "BIGINT") {
                constraints(nullable: "false")
            }

            column(name: "type", type: "VARCHAR(255)") {
                constraints(nullable: "false")
            }
        }
    }

    changeSet(author: "dominic (generated)", id: "1488892755677-9") {
        createTable(tableName: "user") {
            column(autoIncrement: "true", name: "id", type: "BIGINT") {
                constraints(primaryKey: "true", primaryKeyName: "userPK")
            }

            column(name: "version", type: "BIGINT") {
                constraints(nullable: "false")
            }

            column(name: "account_expired", type: "BOOLEAN") {
                constraints(nullable: "false")
            }

            column(name: "account_locked", type: "BOOLEAN") {
                constraints(nullable: "false")
            }

            column(name: "email", type: "VARCHAR(255)") {
                constraints(nullable: "false")
            }

            column(name: "enabled", type: "BOOLEAN") {
                constraints(nullable: "false")
            }

            column(name: "growing_space_id", type: "BIGINT")

            column(name: "password", type: "VARCHAR(255)") {
                constraints(nullable: "false")
            }

            column(name: "password_expired", type: "BOOLEAN") {
                constraints(nullable: "false")
            }

            column(name: "preferred_area_unit", type: "VARCHAR(255)") {
                constraints(nullable: "false")
            }

            column(name: "preferred_length_unit", type: "VARCHAR(255)") {
                constraints(nullable: "false")
            }

            column(name: "preferred_weight_unit", type: "VARCHAR(255)") {
                constraints(nullable: "false")
            }

            column(name: "username", type: "VARCHAR(255)") {
                constraints(nullable: "false")
            }
        }
    }

    changeSet(author: "dominic (generated)", id: "1488892755677-10") {
        createTable(tableName: "user_role") {
            column(name: "user_id", type: "BIGINT") {
                constraints(nullable: "false")
            }

            column(name: "role_id", type: "BIGINT") {
                constraints(nullable: "false")
            }
        }
    }

    changeSet(author: "dominic (generated)", id: "1488892755677-11") {
        addPrimaryKey(columnNames: "user_id, role_id", constraintName: "user_rolePK", tableName: "user_role")
    }

    changeSet(author: "dominic (generated)", id: "1488892755677-12") {
        addUniqueConstraint(columnNames: "authority", constraintName: "UC_ROLEAUTHORITY_COL", tableName: "role")
    }

    changeSet(author: "dominic (generated)", id: "1488892755677-13") {
        addUniqueConstraint(columnNames: "username", constraintName: "UC_USERUSERNAME_COL", tableName: "user")
    }

    changeSet(author: "dominic (generated)", id: "1488892755677-14") {
        addForeignKeyConstraint(baseColumnNames: "user_id", baseTableName: "user_role", constraintName: "FK859n2jvi8ivhui0rl0esws6o", deferrable: "false", initiallyDeferred: "false", referencedColumnNames: "id", referencedTableName: "user")
    }

    changeSet(author: "dominic (generated)", id: "1488892755677-15") {
        addForeignKeyConstraint(baseColumnNames: "crop_id", baseTableName: "area", constraintName: "FK8ievhai93nudxbw9ug8dl5va4", deferrable: "false", initiallyDeferred: "false", referencedColumnNames: "id", referencedTableName: "crop")
    }

    changeSet(author: "dominic (generated)", id: "1488892755677-16") {
        addForeignKeyConstraint(baseColumnNames: "growing_space_harvests_id", baseTableName: "growing_space_harvest", constraintName: "FK96jgpajok9ox1whmpnf9lojar", deferrable: "false", initiallyDeferred: "false", referencedColumnNames: "id", referencedTableName: "growing_space")
    }

    changeSet(author: "dominic (generated)", id: "1488892755677-17") {
        addForeignKeyConstraint(baseColumnNames: "space_id", baseTableName: "area", constraintName: "FK96vqle1gbeehjswuvo5fqb9m8", deferrable: "false", initiallyDeferred: "false", referencedColumnNames: "id", referencedTableName: "growing_space")
    }

    changeSet(author: "dominic (generated)", id: "1488892755677-18") {
        addForeignKeyConstraint(baseColumnNames: "role_id", baseTableName: "user_role", constraintName: "FKa68196081fvovjhkek5m97n3y", deferrable: "false", initiallyDeferred: "false", referencedColumnNames: "id", referencedTableName: "role")
    }

    changeSet(author: "dominic (generated)", id: "1488892755677-19") {
        addForeignKeyConstraint(baseColumnNames: "area_id", baseTableName: "harvest", constraintName: "FKat4splm7l1rcs5fvcbt7p28ly", deferrable: "false", initiallyDeferred: "false", referencedColumnNames: "id", referencedTableName: "area")
    }

    changeSet(author: "dominic (generated)", id: "1488892755677-20") {
        addForeignKeyConstraint(baseColumnNames: "growing_space_id", baseTableName: "user", constraintName: "FKs3w30lt5uwwjtsgoovf8515h5", deferrable: "false", initiallyDeferred: "false", referencedColumnNames: "id", referencedTableName: "growing_space")
    }

    changeSet(author: "dominic (generated)", id: "1488892755677-21") {
        addForeignKeyConstraint(baseColumnNames: "harvest_id", baseTableName: "growing_space_harvest", constraintName: "FKw4audsrjuf3t98jse4m8ybak", deferrable: "false", initiallyDeferred: "false", referencedColumnNames: "id", referencedTableName: "harvest")
    }

    changeSet(author: "dominic (manual)", id: "1488892755677-22") {
        createTable(tableName: "test_table") {
            column(autoIncrement: "true", name: "id", type: "BIGINT") {
                constraints(primaryKey: "true", primaryKeyName: "rolePK")
            }
        }
    }
    changeSet(author: "dominic (manual)", id: "1488892755677-23") {
        dropTable(tableName: "test_table")
    }

    changeSet(author: "dominic (generated)", id: "1491225429206-1") {
        addColumn(tableName: "harvest") {
            column(name: "date_harvested", type: "timestamp") {
                nullable:false
            }
        }
        sql "UPDATE harvest SET date_harvested = date_created"
    }
}


