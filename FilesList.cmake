set(APP ${ROOT}/Application)
set(APP_HEADERS
    ${APP}/Application.h
    ${APP}/Settings.h)
set(APP_SRC
    ${APP}/Application.cpp
    ${APP}/Settings.cpp)

set(CMD ${ROOT}/Commands)
set(CMD_HEADERS
    ${CMD}/CreateEntity.h
    ${CMD}/AddRelation.h
    ${CMD}/RemoveProject.h
    ${CMD}/BaseCommand.h
    ${CMD}/CommandsTypes.h
    ${CMD}/MakeProjectCurrent.h
    ${CMD}/CreateScope.h
    ${CMD}/MoveGraphicObject.h
    ${CMD}/MoveTypeToAnotherScope.h
    ${CMD}/RemoveComponentsCommands.h
    ${CMD}/RenameEntity.h
    ${CMD}/AddComponentsCommands.h
    ${CMD}/UpdateEntity.h
    ${CMD}/OpenProject.h)
set(CMD_SRC
    ${CMD}/CreateEntity.cpp
    ${CMD}/AddRelation.cpp
    ${CMD}/RemoveProject.cpp
    ${CMD}/BaseCommand.cpp
    ${CMD}/MakeProjectCurrent.cpp
    ${CMD}/CreateScope.cpp
    ${CMD}/MoveGraphicObject.cpp
    ${CMD}/MoveTypeToAnotherScope.cpp
    ${CMD}/RemoveComponentsCommands.cpp
    ${CMD}/RenameEntity.cpp
    ${CMD}/AddComponentsCommands.cpp
    ${CMD}/UpdateEntity.cpp
    ${CMD}/OpenProject.cpp)

set(DB ${ROOT}/DB)
set(DB_HEADERS
    ${DB}/Database.h
    ${DB}/ProjectDatabase.h
    ${DB}/IScopeSearcher.h
    ${DB}/ITypeSearcher.h
    ${DB}/DBTypes.hpp)
set(DB_SRC
    ${DB}/ProjectDatabase.cpp
    ${DB}/Database.cpp)

set(ENTITY ${ROOT}/Entity)
set(ENTITY_HEADERS
    ${ENTITY}/field.h
    ${ENTITY}/EntityTypes.hpp
    ${ENTITY}/itypeuser.h
    ${ENTITY}/isectional.h
    ${ENTITY}/EntityFactory.h
    ${ENTITY}/Type.h
    ${ENTITY}/ExtendedType.h
    ${ENTITY}/Enum.h
    ${ENTITY}/Union.h
    ${ENTITY}/Class.h
    ${ENTITY}/TemplateClass.h
    ${ENTITY}/Scope.h
    ${ENTITY}/GraphicEntityData.h
    ${ENTITY}/Property.h
    ${ENTITY}/Template.h
    ${ENTITY}/ClassMethod.h
    ${ENTITY}/TemplateClassMethod.h)
set(ENTITY_SRC
    ${ENTITY}/field.cpp
    ${ENTITY}/isectional.cpp
    ${ENTITY}/EntityFactory.cpp
    ${ENTITY}/Type.cpp
    ${ENTITY}/ExtendedType.cpp
    ${ENTITY}/Enum.cpp
    ${ENTITY}/Union.cpp
    ${ENTITY}/Class.cpp
    ${ENTITY}/TemplateClass.cpp
    ${ENTITY}/Scope.cpp
    ${ENTITY}/GraphicEntityData.cpp
    ${ENTITY}/Property.cpp
    ${ENTITY}/Template.cpp
    ${ENTITY}/ClassMethod.cpp
    ${ENTITY}/TemplateClassMethod.cpp)

set(ENTITY_COMPONENTS ${ENTITY}/Components)
set(ENTITY_COMPONNTS_HEADERS
    ${ENTITY_COMPONENTS}/componentsignatureparser.h
    ${ENTITY_COMPONENTS}/icomponents.h
    ${ENTITY_COMPONENTS}/componentsmaker.h
    ${ENTITY_COMPONENTS}/componentscommon.h
    ${ENTITY_COMPONENTS}/components_types.h
    ${ENTITY_COMPONENTS}/token.h)
set(ENTITY_COMPONNTS_SRC
    ${ENTITY_COMPONENTS}/componentsignatureparser.cpp
    ${ENTITY_COMPONENTS}/icomponents.cpp
    ${ENTITY_COMPONENTS}/componentsmaker.cpp
    ${ENTITY_COMPONENTS}/token.cpp)

set(GEN ${ROOT}/Generator)
set(GEN_HEADERS
    ${GEN}/abstractprojectgenerator.h
    ${GEN}/basiccppprojectgenerator.h
    ${GEN}/virtualdirectory.h
    ${GEN}/virtualfile.h
    ${GEN}/virtualfilesystemabstractitem.h
    ${GEN}/generator_types.hpp)
set(GEN_SRC
    ${GEN}/abstractprojectgenerator.cpp
    ${GEN}/basiccppprojectgenerator.cpp
    ${GEN}/virtualdirectory.cpp
    ${GEN}/virtualfile.cpp
    ${GEN}/virtualfilesystemabstractitem.cpp)

set(GUI ${ROOT}/GUI)
set(GUI_HEADERS
    ${GUI}/addscope.h
    ${GUI}/GuiTypes.hpp
    ${GUI}/scenefilter.h
    ${GUI}/editmethoddialog.h
    ${GUI}/chooseglobaldatabasedialog.h
    ${GUI}/componentseditdelegate.h
    ${GUI}/signatureeditdelegate.h
    ${GUI}/View.h
    ${GUI}/Elements.h
    ${GUI}/MainWindow.h
    ${GUI}/EditEntityDialog.h
    ${GUI}/About.h
    ${GUI}/NewProject.h
    ${GUI}/HtmlDelegate.h
    ${GUI}/Preferences.h
    ${GUI}/EntityProperties.h
    ${GUI}/PropertiesHandlerBase.hpp
    ${GUI}/IPropertiesHandler.hpp
    ${GUI}/Section.h
    ${GUI}/IPropertiesHandler.hpp)
set(GUI_SRC
    ${GUI}/addscope.cpp
    ${GUI}/scenefilter.cpp
    ${GUI}/editmethoddialog.cpp
    ${GUI}/chooseglobaldatabasedialog.cpp
    ${GUI}/componentseditdelegate.cpp
    ${GUI}/signatureeditdelegate.cpp
    ${GUI}/Elements.cpp
    ${GUI}/View.cpp
    ${GUI}/MainWindow.cpp
    ${GUI}/EditEntityDialog.cpp
    ${GUI}/About.cpp
    ${GUI}/NewProject.cpp
    ${GUI}/HtmlDelegate.cpp
    ${GUI}/Preferences.cpp
    ${GUI}/EntityProperties.cpp
    ${GUI}/PropertiesHandlerBase.cpp
    ${GUI}/Section.cpp)

set(GUI_GRAPHICS ${GUI}/graphics)
set(GUI_GRAPHICS_HEADERS
    ${GUI_GRAPHICS}/Entity.h
    ${GUI_GRAPHICS}/GraphicsRelation.h
    ${GUI_GRAPHICS}/Scene.h
    ${GUI_GRAPHICS}/GraphicsTypes.h
    ${GUI_GRAPHICS}/Common.h
    ${GUI_GRAPHICS}/HeaderEditorEventFilter.cpp)
set(GUI_GRAPHICS_SRC
    ${GUI_GRAPHICS}/Entity.cpp
    ${GUI_GRAPHICS}/GraphicsRelation.cpp
    ${GUI_GRAPHICS}/Scene.cpp
    ${GUI_GRAPHICS}/HeaderEditorEventFilter.cpp)

set(HELPERS ${ROOT}/Helpers)
set(HELPERS_HEADERS
    ${HELPERS}/entityhelpres.h
    ${HELPERS}/GeneratorID.h)
set(HELPERS_SRC
    ${HELPERS}/entityhelpres.cpp
    ${HELPERS}/GeneratorID.cpp)

set(MODELS ${ROOT}/Models)
set(MODELS_HEADERS
    ${MODELS}/ProjectTreeModel.h
    ${MODELS}/ApplicationModel.h
    ${MODELS}/BasicTreeItem.h
    ${MODELS}/ModelsTypes.hpp
    ${MODELS}/ComponentsModel.h
    ${MODELS}/MessagesModel.h
    ${MODELS}/SectionalTextConverter.hpp
    ${MODELS}/IMessenger.h)
set(MODELS_SRC
    ${MODELS}/ProjectTreeModel.cpp
    ${MODELS}/ApplicationModel.cpp
    ${MODELS}/BasicTreeItem.cpp
    ${MODELS}/ComponentsModel.cpp
    ${MODELS}/SectionalTextConverter.cpp
    ${MODELS}/MessagesModel.cpp)

set(PROJECT ${ROOT}/Project)
set(PROJECT_HEADERS
    ${PROJECT}/Project.h
    ${PROJECT}/ProjectTypes.hpp)
set(PROJECT_SRC
    ${PROJECT}/Project.cpp)

set(REL ${ROOT}/Relationship)
set(REL_HEADERS
    ${REL}/association.h
    ${REL}/dependency.h
    ${REL}/generalization.h
    ${REL}/multiplyassociation.h
    ${REL}/node.h
    ${REL}/realization.h
    ${REL}/relationship_types.hpp
    ${REL}/RelationFactory.h
    ${REL}/Relation.h)
set(REL_SRC
    ${REL}/association.cpp
    ${REL}/dependency.cpp
    ${REL}/generalization.cpp
    ${REL}/multiplyassociation.cpp
    ${REL}/node.cpp
    ${REL}/realization.cpp
    ${REL}/RelationFactory.cpp
    ${REL}/Relation.cpp)

set(TRANSLATION ${ROOT}/Translation)
set(TRANSLATION_HEADERS
    ${TRANSLATION}/code.h
    ${TRANSLATION}/projecttranslator.h
    ${TRANSLATION}/translator_types.hpp
    ${TRANSLATION}/signaturemaker.h)
set(TRANSLATION_SRC
    ${TRANSLATION}/code.cpp
    ${TRANSLATION}/projecttranslator.cpp
    ${TRANSLATION}/signaturemaker.cpp)

set(UTIL ${ROOT}/Utility)
set(UTIL_HEADERS
    ${UTIL}/helpfunctions.h)
set(UTIL_SRC
    ${UTIL}/helpfunctions.cpp)

set(COMMON ${ROOT}/Common)
set(COMMON_HEADERS
    ${COMMON}/ID.h
    ${COMMON}/BasicElement.h
    ${COMMON}/CommonTypes.hpp
    ${COMMON}/meta.h
    ${COMMON}/ElementsFactory.h
    ${COMMON}/Memento.hpp
    ${COMMON}/SharedFromThis.h)
set(COMMON_SRC
    ${COMMON}/ElementsFactory.cpp
    ${COMMON}/BasicElement.cpp
    ${COMMON}/Memento.cpp
    ${COMMON}/ID.cpp)

set(FREE_HEADERS
    ${ROOT}/enums.h
    ${ROOT}/types.h
    ${ROOT}/Constants.h)
set(FREE_SRC
    ${ROOT}/main.cpp
    ${ROOT}/templates.cpp)

set(BOOST_DI ${ROOT}/boost-di/include)
