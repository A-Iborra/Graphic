// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#define IDICON_G                                                              1
#define IDB_G                                                                 2

#define RCDATA_BUILD_DATE                                                     2

#define  GSYSTEMS_TYPELIB_ID                                                 10
#define  STDOLE_TYPELIB_ID                                                    8
#define  PROPERTYPAGES_TYPELIB_ID                                             6
#define  PROPERTIES_TYPELIB_ID                                                5
#define  EVALUATOR_TYPELIB_ID                                                 4
#define  VARIABLES_TYPELIB_ID                                                 3
#define  FUNCTION_TYPELIB_ID                                                  2

#define IDMENU_GMENU                                                       1000
   #define IDMENU_GFILE                                                    1010
      #define IDMI_GFILE_EXIT                                              1011
   #define IDMENU_GHELP                                                    1020
      #define IDMI_GABOUT                                                  1021

#define IDICON_DATAENGINE                                                  2000
#define IDMENU_DATAENGINE                                                  2001
   #define IDMENU_DATAENGINE_CREATE                                        2002
      #define IDMI_DATAENGINE_CREATE_DATAOBJECT                            2003
      #define IDMI_DATAENGINE_CREATE_GRAPHIC                               2004
      #define IDMI_DATAENGINE_CREATE_OUTPUTFILE                            2005
   #define IDMI_DATAENGINE_START                                           2006
   #define IDMI_DATAENGINE_GRAPH                                           2007
   #define IDMI_DATAENGINE_DELETE                                          2008
   #define IDMI_DATAENGINE_DATAOBJECTS                                     2009
   #define IDMI_DATAENGINE_DATAOBJECTS_MAX                                 2099


#define IDICON_DATAOBJECT                                                  2100
#define IDMENU_DATAOBJECT_FILELIST                                         2101
   #define IDMI_DATAOBJECT_FILELIST_ADD                                    2102
   #define IDMENU_DATAOBJECT_FILELIST_SELECT                               2103
      #define IDMI_DATAOBJECT_FILELIST_SELECT_ALL                          2104
      #define IDMI_DATAOBJECT_FILELIST_DESELECT_ALL                        2105

#define IDMENU_DATAOBJECT_FILELIST_ITEM                                    2108
   #define IDMI_DATAOBJECT_FILELIST_ITEM_REMOVE                            2109
   #define IDMI_DATAOBJECT_FILELIST_ITEM_ANALYSIS                          2110
   #define IDMI_DATAOBJECT_FILELIST_ITEM_RAW                               2111
    

#define IDICON_FUNCTION                                                    4000
#define IDMENU_FUNCTION                                                    4001
   #define IDMENU_FUNCTION_CREATE                                          4002
      #define IDMI_FUNCTION_CREATE_FUNCTION                                4003
      #define IDMI_FUNCTION_CREATE_GRAPHIC                                 4004
      #define IDMI_FUNCTION_CREATE_OUTPUTFILE                              4005
   #define IDMI_FUNCTION_START                                             4006
   #define IDMI_FUNCTION_GRAPH                                             4007
   #define IDMI_FUNCTION_DELETE                                            4008
   #define IDMI_FUNCTION_FUNCTIONS                                         4009
   #define IDMI_FUNCTION_FUNCTIONS_MAX                                     4099


#define IDMENU_GRAPHIC                                                     6000
   #define IDMENU_GRAPHIC_STYLE                                            6005
      #define IDMI_GRAPHIC_MENU_TITLE                                      6006
      #define IDMI_GRAPHIC_SAVE_AS                                         6007
      #define IDMI_GRAPHIC_OPEN                                            6008
      #define IDMI_GRAPHIC_VIEW_2D                                         6010
      #define IDMI_GRAPHIC_VIEW_3D                                         6015
      #define IDMI_GRAPHIC_VIEW_SET                                        6018
      #define IDMI_GRAPHIC_EVALUATE                                        6030
         #define IDMI_GRAPHIC_EVALUATE_FUNCTION                            6031
   #define IDMI_GRAPHIC_ZOOM                                               6130
      #define IDMI_GRAPHIC_ZOOM_IN                                         6131
      #define IDMI_GRAPHIC_ZOOM_OUT                                        6132
      #define IDMI_GRAPHIC_ZOOM_UNDO                                       6133
      #define IDMI_GRAPHIC_ZOOM_FULL                                       6134
   #define IDMI_GRAPHIC_PROPERTIES                                         6140
   #define IDMI_GRAPHIC_LIGHTING                                           6141
   #define IDMI_GRAPHIC_SET_PERSPECTIVE                                    6142
   #define IDMI_GRAPHIC_ERASE                                              6145
   #define IDMI_GRAPHIC_REDRAW                                             6150
   #define IDMI_GRAPHIC_CANCEL                                             6155
   #define IDMI_GRAPHIC_TEXT                                               6160
   #define IDMI_GRAPHIC_PRINT                                              6170


#define IDMENU_AXIS                                                        7000
   #define IDMENU_AXIS_AXIS                                                7001
      #define IDMI_AXIS_SET_PROPERTIES                                     7005

#define IDMENU_PLOT                                                        8000
   #define IDMI_PLOT_PROPERTIES                                            8005
   #define IDMI_PLOT_DELETE                                                8006

#define IDMENU_TEXT                                                        9000
   #define IDMI_TEXT_PROPERTIES                                            9005
   #define IDMI_TEXT_DELETE                                                9006

#define IDMENU_OPENGL                                                     10000
   #define IDMENU_OPENGL_PROPERTIES                                       10001

#define IDDIALOG_DATASET_INSTRUCTIONS                                      2200
   #define IDDI_DATASET_INSTRUCTIONS                                       2201
   #define IDDI_DATASET_INSTRUCTIONS2                                      2202
   #define IDDI_DATASET_INSTRUCTIONS3                                      2203
   #define IDDI_DATASET_INSTRUCTIONS4                                      2204

   #define IDSTRING_DATASET_INSTRUCTIONS                                   2201
   #define IDSTRING_DATASET_INSTRUCTIONS2                                  2202
   #define IDSTRING_DATASET_INSTRUCTIONS3                                  2203
   #define IDSTRING_DATASET_INSTRUCTIONS4                                  2204
   #define IDSTRING_DATASET_DATASOURCE_EMBED_INSTRUCTIONS                  2223

#define IDDIALOG_DATASET_PROPERTIES                                        2210
   #define IDDI_DATASET_NAME                                               2211
   #define IDDI_DATASET_DATASOURCE                                         2212
   #define IDDI_DATASET_GETDATASOURCE                                      2213
   #define IDDI_DATASET_OPEN_WORKBOOK                                      2214
   #define IDDI_DATASET_DATA_BOX                                           2215
   #define IDDI_DATASET_DATA_BOX_ROWCOUNT                                  2216
   #define IDDI_DATASET_DATA_BOX_POPULATE                                  2217
   #define IDDI_DATASET_DATA_BOX_ARITY                                     2218
   #define IDDI_DATASET_DATA_BOX_DIMENSION1_RANGE                          2219
   #define IDDI_DATASET_DATA_BOX_DIMENSION2_RANGE                          2220
   #define IDDI_DATASET_DATA_BOX_DIMENSION3_RANGE                          2221
   #define IDDI_DATASET_DATA_BOX_X_RANGE_FLOOR                             2222
   #define IDDI_DATASET_DATA_BOX_X_RANGE_CEILING                           2223
   #define IDDI_DATASET_DATA_BOX_Y_RANGE_FLOOR                             2224
   #define IDDI_DATASET_DATA_BOX_Y_RANGE_CEILING                           2225
   #define IDDI_DATASET_DATA_BOX_SINGLE_VALUE_LABEL                        2226
   #define IDDI_DATASET_DATA_BOX_SINGLE_VALUE_LABEL2                       2227
   //#define IDDI_DATASET_DATA_BOX_SINGLE_VALUE_LABEL3                       2228
   //#define IDDI_DATASET_DATA_BOX_SINGLE_VALUE_LABEL4                       2229
   #define IDDI_DATASET_DATA_BOX_FLOOR                                     2230
   #define IDDI_DATASET_DATA_BOX_CEILING                                   2231

   #define IDDI_DATASET_EXPORT_BOX                                         2240
   #define IDDI_DATASET_DATASOURCE_EXPORT_DEST                             2241
   #define IDDI_DATASET_GETEXPORT_DEST                                     2242
   #define IDDI_DATASET_DATASOURCE_EXPORT_SHEET_LIST                       2243
   #define IDDI_DATASET_DATASOURCE_EXPORT_CELL                             2244
   #define IDDI_DATASET_DATASOURCE_EXPORT_DEST_OPEN                        2245
   #define IDDI_DATASET_DATASOURCE_EXPORT                                  2246
   #define IDDI_DATASET_DATASOURCE_EXPORT_CANCEL                           2247
   #define IDDI_DATASET_DATASOURCE_EXPORT_STATUS                           2248
   #define IDDI_DATASET_DATASOURCE_EMBED_INSTRUCTIONS                      2249
   #define IDDI_DATASET_DATASOURCE_IS_EMBEDDED                             2250
   #define IDDI_DATASET_DATASOURCE_EMBEDDED_UPDATE                         2251
   #define IDDI_DATASET_DATASOURCE_ERROR                                   2252

#define IDDIALOG_DATASET_EXCEL                                             2230
   #define IDDI_DATASET_EXCEL_SPREADSHEET_LIST                             2231
   #define IDDI_DATASET_EXCEL_SPREADSHEET                                  2232
   #define IDDI_DATASET_EXCEL_OPEN_WORKBOOK                                2233
   #define IDDI_DATASET_EXCEL_NAMEDRANGES_LIST_LABEL                       2234
   #define IDDI_DATASET_EXCEL_WB_NAMEDRANGES_LIST                          2235
   #define IDDI_DATASET_EXCEL_NAMEDRANGES_LABEL                            2236
   #define IDDI_DATASET_EXCEL_NAMEDRANGES_LIST                             2237
   #define IDDI_DATASET_EXCEL_NAMEDRANGE_CONTENTS                          2238
   #define IDDI_DATASET_EXCEL_RANGE_ENTRY                                  2239
   #define IDDI_DATASET_EXCEL_RANGE_INPUT_BOX                              2240
   #define IDDI_DATASET_EXCEL_RANGE_LOAD                                   2241
   #define IDDI_DATASET_EXCEL_SELECTSHEET_LABEL1                           2242
   #define IDDI_DATASET_EXCEL_LOAD_ERROR                                   2243
   #define IDDI_DATASET_EXCEL_HAS_HEADER_ROW_LABEL                         2244
   #define IDDI_DATASET_EXCEL_HAS_HEADER_ROW                               2245

#define IDDIALOG_DATASET_FUNCTION                                          2250
   #define IDDI_DATASET_FUNCTION_RUN                                       2251
   #define IDDI_DATASET_FUNCTION_DATA                                      2252

#define IDDIALOG_DATAOBJECT_FILELIST                                       2250
#define IDDI_DATAOBJECT_FILELIST                                           2251
#define IDDI_DATAOBJECT_FILELIST_ADD                                       2252
#define IDDI_DATAOBJECT_FILELIST_REMOVE                                    2253

#define IDDI_ANALYZER_LEASTSQUARES_NOTES                                   2300
#define IDDI_ANALYZER_LEASTSQUARES_DEGREE                                  2301
#define IDDI_ANALYZER_LEASTSQUARES_3D                                      2302
#define IDDI_ANALYZER_LEASTSQUARES_NOPOINTS                                2303
#define IDDI_ANALYZER_LEASTSQUARES_COEFFICIENTS                            2304
#define IDDI_ANALYZER_LEASTSQUARES_SAMPLE                                  2305
#define IDDI_ANALYZER_LEASTSQUARES_RSS_ERROR                               2306
#define IDDI_ANALYZER_LEASTSQUARES_RMS_ERROR                               2307
#define IDDI_ANALYZER_LSQ_NEEDDEGREE_MSG                                   2308
#define IDDI_ANALYZER_LSQ_NEEDPOINTS_MSG                                   2309
#define IDDI_ANALYZER_LSQ_EQUATION                                         2310

#define IDDI_ANALYZER_QUAIT_NOTES                                          2400
#define IDDI_ANALYZER_QUAIT_NOPOINTS                                       2401
#define IDDI_ANALYZER_QUAIT_NEEDPOINTS_MSG                                 2402

#define IDDI_ANALYZER_DERIVATIVE_NOTES                                     2500
#define IDDI_ANALYZER_DERIVATIVE_POWER                                     2501
#define IDDI_ANALYZER_DERIVATIVE_NOPOINTS                                  2502
#define IDDI_ANALYZER_DERIVATIVE_NEEDPOWER_MSG                             2503
#define IDDI_ANALYZER_DERIVATIVE_NEEDPOINTS_MSG                            2504

#define IDDI_ANALYZER_INTEGRAL_NOTES                                       2600
#define IDDI_ANALYZER_INTEGRAL_NOPOINTS                                    2601
#define IDDI_ANALYZER_INTEGRAL_NEEDPOINTS_MSG                              2602

#define IDOCXBITMAP_FUNCTION                                               4000
#define IDCONTROLBITMAP_FUNCTION                                           4001

#define IDDIALOG_FUNCTION_SPEC                                             4100

   #define IDDI_FUNCTION_PROPERTIES                                        4101  
   #define IDDI_FUNCTION_EQUATION_ENTRY                                    4102
   #define IDDI_FUNCTION_START                                             4105
   #define IDDI_FUNCTION_PAUSE                                             4106
   #define IDDI_FUNCTION_RESUME                                            4107
   #define IDDI_FUNCTION_STOP                                              4108
   #define IDDI_FUNCTION_RESULTS_SPOT                                      4109
   #define IDDI_FUNCTION_RESULT                                            4110
   #define IDDI_FUNCTION_EXPRESSION_LABEL                                  4111
   #define IDDI_FUNCTION_RESULT_LABEL                                      4112
   #define IDDI_FUNCTION_LISTVIEW                                          4113
   //#define IDDI_FUNCTION_PLOT_PROPERTIES                                   4114
   //#define IDDI_FUNCTION_DATASET_PROPERTIES                                4115

   #define IDDI_FUNCTION_SAVEPREP                                          4121
   #define IDDI_FUNCTION_SAVE                                              4122
   #define IDDI_FUNCTION_INIT                                              4123
   #define IDDI_FUNCTION_LOAD                                              4124
   #define IDDI_FUNCTION_ISDIRTY                                           4125

   #define IDSTRING_FUNCTION_INSTRUCTIONS                                  4100
   #define IDSTRING_FUNCTION_DEFAULT1                                      4101
   #define IDSTRING_FUNCTION_DEFAULT2                                      4102
   #define IDSTRING_FUNCTION_DEFAULT3                                      4103
   #define IDSTRING_FUNCTION_DEFAULT4                                      4104
   #define IDSTRING_FUNCTION_DEFAULT5                                      4105
   #define IDSTRING_FUNCTION_DEFAULT6                                      4106
   #define IDSTRING_FUNCTION_DEFAULT7                                      4107
   #define IDSTRING_FUNCTION_DEFAULT8                                      4108
   #define IDSTRING_FUNCTION_DEFAULT9                                      4109
   #define IDSTRING_FUNCTION_DEFAULT10                                     4110

 
#define IDDIALOG_FUNCTION_ERRORS                                           4300
   #define IDDI_FUNCTION_ERRORS_CLEAR                                      4301
   #define IDDI_FUNCTION_ERRORS_BOX                                        4302

#define IDDIALOG_FUNCTION_VARIABLES                                        4400
   #define IDDI_FUNCTION_VARIABLES_TABS                                    4401
   #define IDDI_FUNCTION_VARIABLES_DOMAIN_BOX                              4402

#define IDDIALOG_FUNCTION_PROPERTIES                                       5100

   #define IDDI_FUNCTION_PROPERTIES_START                                  5101
   #define IDDI_FUNCTION_PROPERTIES_EQUATION_ENTRY                         5105
   #define IDDI_FUNCTION_PROPERTIES_VARIABLES                              5106
   #define IDDI_FUNCTION_PROPERTIES_VARIABLE_EDIT                          5107
   #define IDDI_FUNCTION_PROPERTIES_ALLVARIABLES_EDIT                      5108
   //#define IDDI_FUNCTION_PROPERTIES_ALLOWPROPERTIES                        5109
   //#define IDDI_FUNCTION_PROPERTIES_ALLOWCTLVISPROPS                       5110
//
//NTC: 12-28-2017: I am taking control visibility settings out of the interface.
// I don't really recall why there are even there, unless at design time, a developer wants to hide
// the entire Function specifications dialog from the UI (perhaps to prevent the specification dialog(s)
// from appearing at all, which would be easy to implement as one property.)
//
//#define IDDIALOG_FUNCTION_PROPERTIES_VISIBILITY                            5120
//
//   #define IDDI_FUNCTION_PROPERTIES_VISIBILITY_EXPRESSION                  5121
//   #define IDDI_FUNCTION_PROPERTIES_VISIBILITY_RESULTS                     5122
//   #define IDDI_FUNCTION_PROPERTIES_VISIBILITY_VARIABLES                   5123
//   #define IDDI_FUNCTION_PROPERTIES_VISIBILITY_CONTROLS                    5130
//   #define IDDI_FUNCTION_PROPERTIES_VISIBILITY_START                       5131
//   #define IDDI_FUNCTION_PROPERTIES_VISIBILITY_PAUSE                       5132
//   #define IDDI_FUNCTION_PROPERTIES_VISIBILITY_RESUME                      5133
//   #define IDDI_FUNCTION_PROPERTIES_VISIBILITY_STOP                        5134
//   #define IDDI_FUNCTION_PROPERTIES_VISIBILITY_PLOTPROPS                   5135
//   #define IDDI_FUNCTION_PROPERTIES_VISIBILITY_DATASETPROPS                5136

#define IDDIALOG_VARIABLE                                                  6100
#define IDDI_VARIABLE_VALUE_VARIABLE                                       6105
#define IDDI_VARIABLE_INDEPENDENT_VARIABLE                                 6106
#define IDDI_VARIABLE_VALUE                                                6107
#define IDDI_VARIABLE_GROUP_NAME                                           6110
#define IDDI_VARIABLE_DOMAIN_NAME                                          6120
#define IDDI_VARIABLE_DOMAIN_MIN                                           6130
#define IDDI_VARIABLE_DOMAIN_MAX                                           6140
#define IDDI_VARIABLE_STEPS_LABEL                                          6148
#define IDDI_VARIABLE_STEPS                                                6149
#define IDDI_VARIABLE_DOMAIN_EXPRESSION                                    6150
#define IDDI_VARIABLE_DOMAIN_INVALID                                       6160
#define IDDI_VARIABLE_DOMAIN_GROUP_NAME                                    6170
#define IDDI_VARIABLE_PROPERTIES_OK                                        6180
#define IDDI_VARIABLE_PROPERTIES_APPLY                                     6190
#define IDDI_VARIABLE_PROPERTIES_CANCEL                                    6200
#define IDDI_VARIABLE_PROPERTIES_TOFILE                                    6210
#define IDDI_VARIABLE_PROPERTIES_TOGRAPH                                   6220
#define IDDIALOG_VARIABLE_PROPERTIES                                       6201


#define IDDIALOG_TOOLBOX                                                   3000

#define IDDT_TOOLBOX                                                        100
#define IDDI_TOOLBOX_PTR                                                    101
#define IDBT_TOOLBOX_PTR                                                    102
#define IDBM_TOOLBOX_PTR                                                    103

#define IDDI_TOOLBOX_DE                                                     200
#define IDBT_TOOLBOX_DE                                                     201
#define IDBM_TOOLBOX_DE                                                     202
#define IDCU_TOOLBOX_DE                                                     203

#define IDDI_TOOLBOX_FUN                                                    400
#define IDBT_TOOLBOX_FUN                                                    401
#define IDBM_TOOLBOX_FUN                                                    402
#define IDCU_TOOLBOX_FUN                                                    403

#define IDDIALOG_OPENGL_PROPERTIES                                          500
#define IDDI_OPENGL_PROPERTIES_PROP1                                        501
#define IDDI_OPENGL_ALLOWHARDWAREACCELERATION                               502

#define IDOCXBITMAP_PROPERTIES                                             6000
#define IDCONTROLBITMAP_PROPERTIES                                         6001

#define IDDIALOG_PROPERTIES                                                6500
   #define IDDI_PROPERTIES_NOTVISIBLE1                                     6501
   #define IDDI_PROPERTIES_NOTVISIBLE2                                     6502

#define IDDIALOG_PROPERTIES_PROPERTIES                                     6600
   #define IDDI_PROPERTIES_PROPERTIES_FILENAME                             6601
   #define IDDI_PROPERTIES_PROPERTIES_FILEPATH                             6602
   #define IDDI_PROPERTIES_PROPERTIES_EXTENSIONS                           6603
   #define IDDI_PROPERTIES_PROPERTIES_FILESAVEPROMPT                       6604
   #define IDDI_PROPERTIES_PROPERTIES_FILETYPE                             6605
   #define IDDI_PROPERTIES_PROPERTIES_DEBUGGINGENABLED                     6606

#define IDOCXBITMAP_GRAPHIC                                                7000
#define IDCONTROLBITMAP_GRAPHIC                                            7001

#define IDNOTEBOOK_GRAPHIC                                                 7150
#define IDNI_GRAPHIC_NOTEBOOK                                              7151
#define IDNI_GRAPHIC_OK                                                    7152
#define IDNI_GRAPHIC_APPLY                                                 7153
#define IDNI_GRAPHIC_CANCEL                                                7154

#define IDDIALOG_GRAPHIC_SIZEPOS                                           7200
#define IDDI_GRAPHIC_SIZE_MARGINUNITS                                      7203
#define IDDI_GRAPHIC_SIZE_LEFTMARGIN                                       7204
#define IDDI_GRAPHIC_SIZE_RIGHTMARGIN                                      7205
#define IDDI_GRAPHIC_SIZE_BOTTOMMARGIN                                     7206
#define IDDI_GRAPHIC_SIZE_TOPMARGIN                                        7207
#define IDDI_GRAPHIC_SIZE_FITALL                                           7208

#define IDDIALOG_GRAPHIC_STYLE                                             7250
#define IDDI_GRAPHIC_STYLE_SAMPLE                                          7260
#define IDDI_GRAPHIC_SUB_STYLE_NATURAL                                     7261
#define IDDI_GRAPHIC_SUB_STYLE_CONTOUR                                     7262
#define IDDI_GRAPHIC_SUB_STYLE_PIE                                         7263
#define IDDI_GRAPHIC_SUB_STYLE_WIREFRAME                                   7264
#define IDDI_GRAPHIC_SUB_STYLE_SURFACE                                     7265
#define IDDI_GRAPHIC_SUB_STYLE_STACKS                                      7266
#define IDDI_GRAPHIC_STYLE_SETVIEW                                         7267
                                                                           
#define IDDIALOG_GRAPHIC_TEXT_SETTINGS                                     7300
   #define IDDI_GRAPHIC_TEXTS_COUNT                                        7301
   #define IDDI_GRAPHIC_TEXTS_ADD                                          7302
   #define IDDI_GRAPHIC_TEXTS_EDIT                                         7303
   #define IDDI_GRAPHIC_TEXTS_DELETE                                       7304
   #define IDDI_GRAPHIC_TEXTS_LIST                                         7310


#define IDDIALOG_GRAPHIC_LIGHTING                                          7500
#define IDDI_LIGHT_ENABLED                                                 7510
#define IDDI_LIGHT_AMBIENT_RED                                             7511
#define IDDI_LIGHT_AMBIENT_GREEN                                           7512
#define IDDI_LIGHT_AMBIENT_BLUE                                            7513
#define IDDI_LIGHT_AMBIENT_BACKGROUND                                      7514
#define IDDI_LIGHT_DIFFUSE_RED                                             7515
#define IDDI_LIGHT_DIFFUSE_GREEN                                           7516
#define IDDI_LIGHT_DIFFUSE_BLUE                                            7517
#define IDDI_LIGHT_DIFFUSE_BACKGROUND                                      7518
#define IDDI_LIGHT_SPECULAR_RED                                            7519
#define IDDI_LIGHT_SPECULAR_GREEN                                          7520
#define IDDI_LIGHT_SPECULAR_BLUE                                           7521
#define IDDI_LIGHT_SPECULAR_BACKGROUND                                     7522
#define IDDI_LIGHT_POS_X                                                   7523
#define IDDI_LIGHT_POS_Y                                                   7524
#define IDDI_LIGHT_POS_Z                                                   7525
#define IDDI_LIGHT_POS_X_EVALED                                            7526
#define IDDI_LIGHT_POS_Y_EVALED                                            7527
#define IDDI_LIGHT_POS_Z_EVALED                                            7528
#define IDDI_CHOOSE_LIGHT_NO                                               7529
#define IDDI_LIGHT_ENABLED_LABEL                                           7530
#define IDDI_LIGHT_AMBIENT_RED_SPIN                                        7531
#define IDDI_LIGHT_AMBIENT_GREEN_SPIN                                      7532
#define IDDI_LIGHT_AMBIENT_BLUE_SPIN                                       7533
#define IDDI_LIGHT_DIFFUSE_RED_SPIN                                        7534
#define IDDI_LIGHT_DIFFUSE_GREEN_SPIN                                      7535
#define IDDI_LIGHT_DIFFUSE_BLUE_SPIN                                       7536
#define IDDI_LIGHT_SPECULAR_RED_SPIN                                       7537
#define IDDI_LIGHT_SPECULAR_GREEN_SPIN                                     7538
#define IDDI_LIGHT_SPECULAR_BLUE_SPIN                                      7539
#define IDDI_LIGHT_AMBIENT_CHOOSE                                          7540
#define IDDI_LIGHT_DIFFUSE_CHOOSE                                          7541
#define IDDI_LIGHT_SPECULAR_CHOOSE                                         7542
                                                                           
#define IDDIALOG_GRAPHIC_BACKGROUND                                        7600
#define IDDI_LIGHT_BACKGROUND_RED                                          7601
#define IDDI_LIGHT_BACKGROUND_GREEN                                        7602
#define IDDI_LIGHT_BACKGROUND_BLUE                                         7603
#define IDDI_LIGHT_BACKGROUND_RED_SPIN                                     7604
#define IDDI_LIGHT_BACKGROUND_GREEN_SPIN                                   7605
#define IDDI_LIGHT_BACKGROUND_BLUE_SPIN                                    7606
#define IDDI_LIGHT_BACKGROUND_CHOOSE                                       7607
#define IDDI_LIGHT_BACKGROUND_BACKGROUND                                   7608

#define IDDIALOG_GRAPHIC_PLOTS                                             7700
#define IDDI_GRAPHIC_PLOTS_INSTRUCTIONS                                    7701
#define IDDI_GRAPHIC_PLOTS_COUNT                                           7710
#define IDDI_GRAPHIC_PLOTS_EDIT                                            7730
#define IDDI_GRAPHIC_PLOTS_GRAPHIC                                         7750
#define IDDI_GRAPHIC_PLOTS_LIST                                            7760
#define IDDI_GRAPHIC_PLOTS_DEFAULTS_BOX                                    7761
#define IDDI_GRAPHIC_PLOTS_ARITY_TABS                                      7762
#define IDDI_GRAPHIC_PLOTS_2D_TYPES_SCROLL_PANE                            7763
#define IDDI_GRAPHIC_PLOTS_2D_TYPES_SCROLL_BAR                             7764
#define IDDI_GRAPHIC_PLOTS_3D_TYPES_SCROLL_PANE                            7765
#define IDDI_GRAPHIC_PLOTS_3D_TYPES_SCROLL_BAR                             7766
#define IDDI_GRAPHIC_PLOTS_GROUP_ARITY                                     7770
#define IDDI_GRAPHIC_PLOTS_GROUP_PLOTTYPE                                  7780

#define IDDIALOG_GRAPHIC_FUNCTIONS                                         7800
#define IDDI_GRAPHIC_FUNCTIONS_COUNT                                       7805
#define IDDI_GRAPHIC_FUNCTIONS_ADD                                         7810
#define IDDI_GRAPHIC_FUNCTIONS_EDIT                                        7820
#define IDDI_GRAPHIC_FUNCTIONS_DELETE                                      7830
#define IDDI_GRAPHIC_FUNCTIONS_LIST                                        7840
#define IDDI_GRAPHIC_FUNCTIONS_ALLOWCTLVISPROPS                            7850

#define IDDIALOG_GRAPHIC_AXIIS                                             7900
#define IDDI_GRAPHIC_AXIIS_USE_OPENGL                                      7901
#define IDDI_GRAPHIC_AXIIS_XAXIS_TEXT                                      7910
#define IDDI_GRAPHIC_AXIIS_XAXIS_EDIT                                      7915
#define IDDI_GRAPHIC_AXIIS_YAXIS_TEXT                                      7920
#define IDDI_GRAPHIC_AXIIS_YAXIS_EDIT                                      7925
#define IDDI_GRAPHIC_AXIIS_ZAXIS_TEXT                                      7930
#define IDDI_GRAPHIC_AXIIS_ZAXIS_EDIT                                      7935

#define IDDIALOG_GRAPHIC_TEXT_ENTRY                                        7950
#define IDDI_GRAPHIC_TEXT_TEXT                                             7960
#define IDDI_GRAPHIC_TEXT_OK                                               7970
#define IDDI_GRAPHIC_TEXT_CANCEL                                           7980

#define IDNOTEBOOK_GRAPHIC_DATASOURCES                                     8000
#define IDDI_DATASOURCES_TAB                                               8010
#define IDDI_DATASOURCES_FUNCTIONS_TAB                                     8020
#define IDDI_DATASOURCES_AUTOCLEAR                                         8030
#define IDDI_DATASOURCES_CLEAR                                             8031
#define IDDI_DATASOURCES_RESETDOMAIN                                       8040
#define IDDI_DATASOURCES_DATASETS_TAB                                      8050

#define IDNOTEBOOK_AXIS                                                    8100
#define IDNI_AXIS_NOTEBOOK                                                 8101
#define IDNI_AXIS_OK                                                       8102
#define IDNI_AXIS_APPLY                                                    8103
#define IDNI_AXIS_CANCEL                                                   8104

#define IDDIALOG_AXIS_STYLE                                                8200
#define IDDI_AXIS_STYLE_LINEWEIGHT                                         8201
#define IDDI_AXIS_STYLE_LINEWEIGHT_SPIN                                    8202
#define IDDI_AXIS_STYLE_LINEWEIGHT_UNITS                                   8203
#define IDDI_AXIS_STYLE_LINESTYLES                                         8204
#define IDDI_AXIS_STYLE_CHANGELINECOLOR                                    8205
#define IDDI_AXIS_STYLE_TICKSABOVE                                         8206
#define IDDI_AXIS_STYLE_TICKSACCROSS                                       8207
#define IDDI_AXIS_STYLE_TICKSBELOW                                         8208
#define IDDI_AXIS_STYLE_MSG_PCNTABOVE                                      8209
#define IDDI_AXIS_STYLE_MSG_GRIDSPERTICK                                   8212
#define IDDI_AXIS_STYLE_GRIDSPERTICK                                       8213
#define IDDI_AXIS_STYLE_PCNTABOVE                                          8214
#define IDDI_AXIS_STYLE_TICKLENGTH                                         8215
#define IDDI_AXIS_STYLE_TICKLENGTH_UNITS                                   8216
#define IDDI_AXIS_STYLE_LINEWEIGHTS                                        8217
#define IDDI_AXIS_STYLE_DETERMINESDOMAIN                                   7220
#define IDDI_AXIS_STYLE_MSG_TICKCOUNT                                      8221
#define IDDI_AXIS_STYLE_TICKCOUNT                                          8222
                                                                             
#define IDDIALOG_AXIS_POSITION                                             8300
#define IDDI_AXIS_POSITION_ORIGIN_MINPOINT                                 8302
#define IDDI_AXIS_POSITION_ORIGIN_X                                        8305
#define IDDI_AXIS_POSITION_ORIGIN_Y                                        8306
#define IDDI_AXIS_POSITION_ORIGIN_Z                                        8307
#define IDDI_AXIS_POSITION_ENDPOINT_MAXPOINT                               8312
#define IDDI_AXIS_POSITION_ENDPOINT_X                                      8315
#define IDDI_AXIS_POSITION_ENDPOINT_Y                                      8316
#define IDDI_AXIS_POSITION_ENDPOINT_Z                                      8317
#define IDDI_AXIS_POSITION_NOTES                                           8320

#define IDDIALOG_AXIS_TEXT                                                 8350
#define IDDI_AXIS_TEXT_LABELTEXT                                           8351
#define IDDI_AXIS_TEXT_LABEL                                               8352
#define IDDI_AXIS_TEXT_DISPLAYTICKLABELS                                   8353
#define IDDI_AXIS_TEXT_AUTOPRECISION                                       8354
#define IDDI_AXIS_TEXT_SPECIFICPRECISION                                   8355
#define IDDI_AXIS_TEXT_AXISTICKLABELSIZE                                   8356
#define IDDI_AXIS_TEXT_TEXT1                                               8357
#define IDDI_AXIS_TEXT_AXISTICKLABELPRECISION_VALUE                        8358
#define IDDI_AXIS_TEXT_AXISPRECISIONSPIN                                   8359

#define IDDI_AXIS_TEXT_LABEL_POSITION_NATURAL                              8370
#define IDDI_AXIS_TEXT_LABEL_POSITION_X                                    8371
#define IDDI_AXIS_TEXT_LABEL_POSITION_Y                                    8372
#define IDDI_AXIS_TEXT_LABEL_POSITION_Z                                    8373
#define IDDI_AXIS_TEXT_LABEL_ANGLE_PERPENDICULAR                           8374
#define IDDI_AXIS_TEXT_LABEL_ANGLE_PARALLEL                                8375
#define IDDI_AXIS_TEXT_LABEL_ANGLE_ISOTHER                                 8376
#define IDDI_AXIS_TEXT_LABEL_ANGLE_OTHER                                   8377

#define IDDI_AXIS_TEXT_TICK_LABEL_SIZE                                     8380
#define IDDI_AXIS_TEXT_TICK_LABEL_SIZE_UNITS                               8381

#define IDDIALOG_AXIS_COLOR                                                8400

#define IDDI_AXIS_COLOR_LINE_COLOR_RED                                     8401
#define IDDI_AXIS_COLOR_LINE_COLOR_GREEN                                   8402
#define IDDI_AXIS_COLOR_LINE_COLOR_BLUE                                    8403
#define IDDI_AXIS_COLOR_LINE_COLOR_RED_SPIN                                8404
#define IDDI_AXIS_COLOR_LINE_COLOR_GREEN_SPIN                              8405
#define IDDI_AXIS_COLOR_LINE_COLOR_BLUE_SPIN                               8406
#define IDDI_AXIS_COLOR_LINE_COLOR_CHOOSE                                  8407
#define IDDI_AXIS_COLOR_LINE_COLOR_BACKGROUND                              8408

#define IDDI_AXIS_COLOR_TICK_LABEL_COLOR_TRACK                             8409
#define IDDI_AXIS_COLOR_TICK_LABEL_COLOR_RED                               8410
#define IDDI_AXIS_COLOR_TICK_LABEL_COLOR_GREEN                             8411
#define IDDI_AXIS_COLOR_TICK_LABEL_COLOR_BLUE                              8412
#define IDDI_AXIS_COLOR_TICK_LABEL_COLOR_RED_SPIN                          8413
#define IDDI_AXIS_COLOR_TICK_LABEL_COLOR_GREEN_SPIN                        8414
#define IDDI_AXIS_COLOR_TICK_LABEL_COLOR_BLUE_SPIN                         8415
#define IDDI_AXIS_COLOR_TICK_LABEL_COLOR_BACKGROUND                        8416
#define IDDI_AXIS_COLOR_TICK_LABEL_COLOR_CHOOSE                            8417

#define IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_TRACK                             8419
#define IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_RED                               8420
#define IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_GREEN                             8421
#define IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_BLUE                              8422
#define IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_RED_SPIN                          8423
#define IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_GREEN_SPIN                        8424
#define IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_BLUE_SPIN                         8425
#define IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_BACKGROUND                        8426
#define IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_CHOOSE                            8427


#define IDDIALOG_PLOT_DIMENSION                                            9250
   #define IDDI_PLOT_DIMENSION_OVERRIDE_OWNER                              9251
   #define IDDI_PLOT_DIMENSION_3D                                          9252
   #define IDDI_PLOT_DIMENSION_2D                                          9253
   #define IDDI_PLOT_DIMENSION_SAMPLEPOSITION                              9254

#define IDDIALOG_PLOT_TYPE                                                 9270
   #define IDDI_PLOT_TYPE_OVERRIDE_OWNER                                   9271
   #define IDDI_PLOT_TYPE_ARITY_TABS                                       9272
   #define IDDI_PLOT_TYPE_2D_TYPES_SCROLL_PANE                             9273
   #define IDDI_PLOT_TYPE_3D_TYPES_SCROLL_PANE                             9274
   #define IDDI_PLOT_TYPE_2D_TYPES_SCROLL_BAR                              9275
   #define IDDI_PLOT_TYPE_3D_TYPES_SCROLL_BAR                              9276
   #define IDDI_PLOT_TYPE_NONE_SELECTED                                    9277

   #define IDDI_PLOT_TYPE_2D_MIN                                           9280
   #define IDDI_PLOT_TYPE_2D_MAX                                           9320

   #define IDDI_PLOT_TYPE_3D_MIN                                           9321
   #define IDDI_PLOT_TYPE_3D_MAX                                           9361

#define IDDIALOG_PLOT_COLOR                                                9200
   #define IDDI_PLOT_COLOR_GROUP_LINE                                      9210
   #define IDDI_PLOT_COLOR_LINE_BACKGROUND                                 9211
   #define IDDI_PLOT_COLOR_LINE_RED                                        9212
   #define IDDI_PLOT_COLOR_LINE_GREEN                                      9213
   #define IDDI_PLOT_COLOR_LINE_BLUE                                       9214
   #define IDDI_PLOT_COLOR_LINE_RED_SPIN                                   9215
   #define IDDI_PLOT_COLOR_LINE_GREEN_SPIN                                 9216
   #define IDDI_PLOT_COLOR_LINE_BLUE_SPIN                                  9217
   #define IDDI_PLOT_COLOR_LINE_CHOOSE                                     9218

   #define IDDI_PLOT_COLOR_GROUP_TOP                                       9220
   #define IDDI_PLOT_COLOR_TOP_BACKGROUND                                  9221
   #define IDDI_PLOT_COLOR_TOP_RED                                         9222
   #define IDDI_PLOT_COLOR_TOP_GREEN                                       9223
   #define IDDI_PLOT_COLOR_TOP_BLUE                                        9224
   #define IDDI_PLOT_COLOR_TOP_RED_SPIN                                    9225
   #define IDDI_PLOT_COLOR_TOP_GREEN_SPIN                                  9226
   #define IDDI_PLOT_COLOR_TOP_BLUE_SPIN                                   9227
   #define IDDI_PLOT_COLOR_TOP_CHOOSE                                      9228

   #define IDDI_PLOT_COLOR_GROUP_BOTTOM                                    9230
   #define IDDI_PLOT_COLOR_BOTTOM_BACKGROUND                               9231
   #define IDDI_PLOT_COLOR_BOTTOM_RED                                      9232
   #define IDDI_PLOT_COLOR_BOTTOM_GREEN                                    9233
   #define IDDI_PLOT_COLOR_BOTTOM_BLUE                                     9234
   #define IDDI_PLOT_COLOR_BOTTOM_RED_SPIN                                 9235
   #define IDDI_PLOT_COLOR_BOTTOM_GREEN_SPIN                               9236
   #define IDDI_PLOT_COLOR_BOTTOM_BLUE_SPIN                                9237
   #define IDDI_PLOT_COLOR_BOTTOM_CHOOSE                                   9238


//
// The following is for the properties page presented by the GraphicControl
// 
#define IDDIALOG_GRAPHIC_DATASETS                                          9300
#define IDDI_GRAPHIC_DATASETS_COUNT                                        9310
#define IDDI_GRAPHIC_DATASETS_ADD                                          9320
#define IDDI_GRAPHIC_DATASETS_EDIT                                         9330
#define IDDI_GRAPHIC_DATASETS_DELETE                                       9340
#define IDDI_GRAPHIC_DATASETS_LIST                                         9350
#define IDDI_GRAPHIC_DATASETS_ALLOWCTLVISPROPS                             9360

//
// These constants are for the dialog the DataSet object shows in the run-time
// UI in the DataSources specifications window.
//
#define IDDIALOG_DATASET_SPEC                                              9400
   #define IDDI_DATASET_SPEC_PROPERTIES                                    9410
   #define IDDI_DATASET_SPEC_NAME                                          9411
   #define IDDI_DATASET_SPEC_DATASOURCE                                    9412
   //#define IDDI_DATASET_SPEC_PLOT_PROPERTIES                               9430
   #define IDDI_DATASET_SPEC_PLOT                                          9431

#define IDDIALOG_VIEWSET                                                  10000
#define IDDI_VIEWSET_XAXIS                                                10004
#define IDDI_VIEWSET_YAXIS                                                10005
#define IDDI_VIEWSET_ZAXIS                                                10006
#define IDDI_VIEWSET_PHISET                                               10007
#define IDDI_VIEWSET_THETASET                                             10008
#define IDDI_VIEWSET_SPINSET                                              10009
#define IDDI_VIEWSET_ELEVATION_TEXT                                       10010
#define IDDI_VIEWSET_PHITEXT                                              10011
#define IDDI_VIEWSET_PHITEXT_MIN                                          10012
#define IDDI_VIEWSET_PHITEXT_MAX                                          10013
#define IDDI_VIEWSET_THETATEXT                                            10014
#define IDDI_VIEWSET_THETATEXT_MIN                                        10015
#define IDDI_VIEWSET_THETATEXT_MAX                                        10016
#define IDDI_VIEWSET_SPINTEXT                                             10017
#define IDDI_VIEWSET_SPINTEXT_MIN                                         10018
#define IDDI_VIEWSET_SPINTEXT_MAX                                         10019
#define IDDI_VIEWSET_VIEW2D                                               10020

#define IDDIALOG_ABOUT                                                    11000
#define IDDI_G_ABOUT_BUTTON                                               11135
#define IDDI_G_ABOUT_TEXT1                                                11136
#define IDDI_G_ABOUT_TEXT2                                                11137
#define IDDI_G_ABOUT_TEXT3                                                11138
#define IDDI_G_ABOUT_ADDRESS                                              11139
#define IDDI_G_ABOUT_TEXT4                                                11140
#define IDDI_G_ABOUT_TEXT5                                                11141
#define IDDI_G_ABOUT_TEXT6                                                11142
#define IDDI_G_ABOUT_TEXT7                                                11143
#define IDDI_G_ABOUT_GETTING_STARTED                                      11144
#define IDDI_G_ABOUT_PANEL_0                                              11145
#define IDDI_G_ABOUT_PANEL_1                                              11146
#define IDDI_G_ABOUT_PANEL_2                                              11147

#define IDNOTEBOOK_TEXT                                                   12000
#define IDNI_TEXT_NOTEBOOK                                                12010
#define IDNI_TEXT_OK                                                      12020
#define IDNI_TEXT_APPLY                                                   12030
#define IDNI_TEXT_DEFAULT                                                 12040
#define IDNI_TEXT_CANCEL                                                  12050

#define IDDIALOG_TEXT_CONTENT                                             12100
#define IDDI_TEXT_TEXT                                                    12110
#define IDDI_TEXT_DESCRIPTION                                             12111

#define IDDIALOG_TEXT_STYLE                                               12200
#define IDDI_TEXT_RENDEROPENGL                                            12205
#define IDDI_TEXT_FONTLIST                                                12210
#define IDDI_TEXT_FONTSTYLELIST                                           12220
#define IDDI_TEXT_FONTSIZE                                                12230
#define IDDI_TEXT_FONTSIZE_SPINNER                                        12231
#define IDDI_TEXT_FONTSIZEUNITSLIST                                       12232

#define IDDIALOG_TEXT_ORIENTATION                                         12300
#define IDDI_TEXT_XYPLANE                                                 12311
#define IDDI_TEXT_XZPLANE                                                 12312
#define IDDI_TEXT_YXPLANE                                                 12313
#define IDDI_TEXT_YZPLANE                                                 12314
#define IDDI_TEXT_ZXPLANE                                                 12315
#define IDDI_TEXT_ZYPLANE                                                 12316
#define IDDI_TEXT_FLIP_LR                                                 12317
#define IDDI_TEXT_FLIP_TB                                                 12318

#define IDDI_TEXT_SCREENPLANE                                             12320
#define IDDI_TEXT_PLANEHEIGHT                                             12321
#define IDDI_TEXT_XCOORDINATE                                             12322
#define IDDI_TEXT_YCOORDINATE                                             12323
#define IDDI_TEXT_ZCOORDINATE                                             12324
#define IDDI_TEXT_BACKSIDE                                                12325
#define IDDI_TEXT_FORMAT_FROM_TOP                                         12330
#define IDDI_TEXT_FORMAT_FROM_CENTER                                      12331
#define IDDI_TEXT_FORMAT_FROM_BOTTOM                                      12332
#define IDDI_TEXT_FORMAT_LEFT                                             12333
#define IDDI_TEXT_FORMAT_CENTER                                           12334
#define IDDI_TEXT_FORMAT_RIGHT                                            12335

#define IDDI_TEXT_TEXTCOLOR_BACKGROUND                                    12360
#define IDDI_TEXT_TEXTCOLOR_RED                                           12361
#define IDDI_TEXT_TEXTCOLOR_GREEN                                         12362
#define IDDI_TEXT_TEXTCOLOR_BLUE                                          12363

#define IDDI_TEXT_TEXTCOLOR_RED_SPIN                                      12374
#define IDDI_TEXT_TEXTCOLOR_GREEN_SPIN                                    12375
#define IDDI_TEXT_TEXTCOLOR_BLUE_SPIN                                     12376

#define IDDI_TEXT_TEXTCOLOR_CHOOSE                                        12381


#define ID_GRAPHICMENU_WORKSPACEMENU                                      20001
#define ID_PLOTMENU_WORKSPACEMENU                                         20002

#define IDBD_DEFAULT_FONT                                                 30001
#define IDBD_ROCKFONT_FONT                                                30002
#define IDBD_SORORITY_FONT                                                30003

#define IDDI_STATIC                                                          -1


#define COLOR_ENTRY_FIELD_WIDTH     32
#define COLOR_ENTRY_FIELD_HEIGHT    14
