# Copyright 2021, 2022 Million Framework Project
# Licensed under BSD-3-Clause. See LICENCE.md for more information.

import bpy
import os
from bpy.props import (StringProperty,
                       BoolProperty,
                       IntProperty,
                       FloatProperty,
                       FloatVectorProperty,
                       EnumProperty,
                       PointerProperty,
                       )
from bpy.types import (Panel,
                       Menu,
                       Operator,
                       PropertyGroup,
                       )


class MIRIWAKU_PR_properties(PropertyGroup):
    my_string = StringProperty(
        name="User Input",
        description=":",
        default="",
        maxlen=1024,
        )

class MIRIWAKU_PT_main_panel(Panel):
    bl_label = "Million Framework Tools"
    bl_idname = "ADDONNAME_PT_main_panel"
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'UI'
    bl_category = "Miriwaku"
 
    def draw(self, context):
        layout = self.layout
        layout.operator("miriwaku.remove_empties")
        layout.operator("miriwaku.fix_materials")
        layout.operator("miriwaku.export_outfit")
        layout.operator("miriwaku.process_head")
        layout.operator("miriwaku.export_head")
        layout.operator("miriwaku.export_accessories")
        layout.operator("miriwaku.rename_face_mt")
        
class MIRIWAKU_OP_name(Operator):
    bl_label = "Name"
    bl_idname = "miriwaku.object_name"

class MIRIWAKU_OP_remove_empties(Operator):
    bl_label = "Remove Empty"
    bl_idname = "miriwaku.remove_empties"

    @classmethod
    def description(cls, context, properties):
        return "Remove all Empty objects in the scene. Make sure you apply all transforms first"
    
    def execute(self, context):    
        for objs in bpy.context.scene.objects:
            if objs.type == 'EMPTY':
                bpy.data.objects.remove(objs)
        self.report({'INFO'}, "Empty objects removed.")
        
        return {'FINISHED'}

class MIRIWAKU_OP_fix_materials(Operator):
    bl_label = "Fix Materials"
    bl_idname = "miriwaku.fix_materials"
    
    @classmethod
    def poll(cls, context):
        return context.object is not None
    
    @classmethod
    def description(cls, context, properties):
        return 'Fix materials to be Opaque. It doesn\'t matter when brought to UE, but it\'ll be nicer to look at'
    
    def execute(self, context):
        does_have_mesh = False
        changed_mat = 0
        for obj in bpy.context.selected_objects:
            if obj.active_material.blend_method != 'OPAQUE':
                changed_mat += 1
                obj.active_material.blend_method = 'OPAQUE'
            else:
                continue
            
            if changed_mat == 0:
                self.report({'WARNING'}, "No changes were made.")
            else:
                self.report({'INFO'}, "Shape key names changed.")

            return {'FINISHED'}

class MIRIWAKU_OP_export_outfit(Operator):
    bl_label = "Export Outfit"
    bl_idname = "miriwaku.export_outfit"
    
    @classmethod
    def poll(cls, context):
        return context.object is not None
    
    @classmethod
    def description(cls, context, properties):
        return 'Export selected body object into an FBX file'
    
    def execute(self, context):
        does_have_armature = False
        does_have_mesh = False
        does_have_armature_mod = False
        armature_count = 0
        skel_name = ''
        for obj in bpy.context.selected_objects:
            if obj.type == 'ARMATURE':
                if obj.name == 'Armature' or obj.name == 'armature':
                    self.report({'ERROR'}, "Object named Armature detected! The script will automatically handle the renaming, therefore no need for naming it Armature manually.")
                    return {'FINISHED'}
                does_have_armature = True
                armature_count = armature_count + 1
                if armature_count > 1:
                    self.report({'ERROR'}, "More than one armature object is selected! For simplicity, it's best to export just one armature.")
                    return {'FINISHED'}
                else:
                    skel_name = obj.name
            if obj.type == 'MESH':
                if 'body' not in obj.name:
                    self.report({'ERROR'}, "Selected mesh is not a body mesh!")
                    return {'FINISHED'}
                for modifier in obj.modifiers:
                    if modifier.type == "ARMATURE":
                        does_have_armature_mod = True
                        if obj.modifiers[0].object.name != skel_name:
                            self.report({'ERROR'}, "Selected mesh is not tied to the selected armature!")
                            return {'FINISHED'}
                if not does_have_armature_mod:
                    self.report({'ERROR'}, "Selected mesh is not bound to any armature!\nMake sure to check if the mesh has the Armature modifier.")
                    return {'FINISHED'}
                        
                does_have_mesh = True
                
        if does_have_armature == False:
            self.report({'ERROR'}, "You must select an armature object!")
            return {'FINISHED'}
        if does_have_mesh == False:
            self.report({'ERROR'}, "At least a mesh object must be selected! Unreal Engine cannot import FBX containing just the armature.")
            return {'FINISHED'}
        
        export_path = os.path.join(bpy.path.abspath('//'), 'SourceAsset', 'Body', bpy.context.object.users_collection[0].name)
        if not os.path.exists(export_path):
            os.makedirs(export_path)

        for obj in bpy.context.selected_objects:
            if obj.name == skel_name and obj.type == 'ARMATURE':
                obj.name = "Armature"

        file_export_path = os.path.join(export_path, "SK_%s.fbx" % (skel_name))

        bpy.ops.export_scene.fbx(
            filepath=file_export_path,
            check_existing=True, 
            filter_glob='*.fbx', 
            use_selection=True, 
            use_active_collection=False, 
            global_scale=1.0, 
            apply_unit_scale=True, 
            apply_scale_options='FBX_SCALE_NONE', 
            use_space_transform=True, 
            bake_space_transform=False, 
            object_types={'ARMATURE', 'MESH', 'OTHER'}, 
            use_mesh_modifiers=False, 
            mesh_smooth_type='FACE', 
            use_subsurf=False, 
            use_mesh_edges=False, 
            use_tspace=False, 
            use_custom_props=False, 
            add_leaf_bones=False, 
            primary_bone_axis='Y', 
            secondary_bone_axis='X', 
            use_armature_deform_only=True, 
            armature_nodetype='NULL', 
            bake_anim=False, 
            path_mode='AUTO', 
            embed_textures=False, 
            batch_mode='OFF', 
            use_batch_own_dir=True, 
            use_metadata=True, 
            axis_forward='-Z', 
            axis_up='Y')
        
        for obj in bpy.context.selected_objects:
            if obj.name == "Armature" and obj.type == 'ARMATURE':
                obj.name = skel_name

        self.report({'INFO'}, "Export completed.")

        return {'FINISHED'}



class MIRIWAKU_OP_process_head(Operator):
    bl_label = "Process Head Mesh"
    bl_idname = "miriwaku.export_head"
    
    @classmethod
    def poll(cls, context):
        return context.object is not None
    
    @classmethod
    def description(cls, context, properties):
        return 'Process the head mesh before exporting.\nThis will remove root bone before KUBI (neck bone).'
    
    def execute(self, context):
        does_have_eyes = False
        does_have_face = False
        does_have_hair = False
        skel_name = ''

        obj = bpy.context.object
        if obj.type == 'ARMATURE':
            if obj.name == 'Armature' or obj.name == 'armature':
                self.report({'WARNING'}, "Object named Armature detected! The tool will automatically handle the renaming, therefore naming it Armature is not necessary.")
            obj.name = str.join(bpy.context.object.users_collection[0].name, 'Head')
            skel_name = obj.name
            for meshes in obj.children:
                if obj.type == 'MESH':
                    if 'eyes' in obj.name:
                        does_have_eyes = True
                    if 'face' in obj.name:
                        does_have_face = True
                    if 'hair' in obj.name:
                        does_have_hair = True
        else:
            self.report({'ERROR'}, "Selected object is not armature type!")

        if not does_have_eyes and not does_have_face and not does_have_hair:
            self.report({'ERROR'}, "Selected armature does not have complete head mesh set!")
            return {'FINISHED'}

        return {'FINISHED'}



class MIRIWAKU_OP_export_head(Operator):
    bl_label = "Export Head"
    bl_idname = "miriwaku.export_head"
    
    @classmethod
    def poll(cls, context):
        return context.object is not None
    
    @classmethod
    def description(cls, context, properties):
        return 'Export selected body object into an FBX file'
    
    def execute(self, context):
        does_have_armature = False
        does_have_mesh = False
        does_have_eyes = False
        does_have_face = False
        does_have_hair = False
        does_have_accs = False
        does_have_armature_mod = False
        armature_count = 0
        skel_name = ''
        for obj in bpy.context.selected_objects:
            if obj.type == 'ARMATURE':
                if obj.name == 'Armature' or obj.name == 'armature':
                    self.report({'ERROR'}, "Object named Armature detected! The script will automatically handle the renaming, therefore no need for naming it Armature manually.")
                    return {'FINISHED'}
                does_have_armature = True
                armature_count = armature_count + 1
                if armature_count > 1:
                    self.report({'ERROR'}, "More than one armature object is selected! For simplicity, it's best to export just one armature.")
                    return {'FINISHED'}
                else:
                    skel_name = obj.name
            if obj.type == 'MESH':
                if 'eyes' in obj.name:
                    does_have_eyes = True
                if 'face' in obj.name:
                    does_have_face = True
                if 'hair' in obj.name:
                    does_have_hair = True
                if 'acc' in obj.name:
                    does_have_accs = True
                    self.report({'ERROR'}, "Accessories mesh is selected!\nFor simplicity, accessories should be separate skeletal meshes in Miriwaku - use Export Accessories for accessories instead.")
                    return {'FINISHED'}
                for modifier in obj.modifiers:
                    if modifier.type == "ARMATURE":
                        does_have_armature_mod = True
                        if obj.modifiers[0].object.name != skel_name:
                            self.report({'ERROR'}, "One or more of selected meshes are not tied to the selected armature!")
                            return {'FINISHED'}
                if not does_have_armature_mod:
                    self.report({'ERROR'}, "One or more of selected meshes are not bound to any armature!\nMake sure to check if they have the Armature modifier.")
                    return {'FINISHED'}
                        
                does_have_mesh = True
                
        if does_have_armature == False:
            self.report({'ERROR'}, "You must select an armature object!")
            return {'FINISHED'}
        if does_have_mesh == False:
            self.report({'ERROR'}, "At least a mesh object must be selected! Unreal Engine cannot import FBX containing just the armature.")
            return {'FINISHED'}
        if not does_have_eyes or not does_have_face or not does_have_hair:
            self.report({'ERROR'}, "Head object selection is incomplete!")
            return {'FINISHED'}
        
        export_path = os.path.join(bpy.path.abspath('//'), 'SourceAsset', 'Head', bpy.context.object.users_collection[0].name)
        if not os.path.exists(export_path):
            os.makedirs(export_path)

        for obj in bpy.context.selected_objects:
            if obj.name == skel_name and obj.type == 'ARMATURE':
                obj.name = "Armature"

        file_export_path = os.path.join(export_path, "SK_%s.fbx" % (skel_name))

        bpy.ops.export_scene.fbx(
            filepath=file_export_path,
            check_existing=True, 
            filter_glob='*.fbx', 
            use_selection=True, 
            use_active_collection=False, 
            global_scale=1.0, 
            apply_unit_scale=True, 
            apply_scale_options='FBX_SCALE_NONE', 
            use_space_transform=True, 
            bake_space_transform=False, 
            object_types={'ARMATURE', 'MESH', 'OTHER'}, 
            use_mesh_modifiers=False, 
            mesh_smooth_type='FACE', 
            use_subsurf=False, 
            use_mesh_edges=False, 
            use_tspace=False, 
            use_custom_props=False, 
            add_leaf_bones=False, 
            primary_bone_axis='Y', 
            secondary_bone_axis='X', 
            use_armature_deform_only=True, 
            armature_nodetype='NULL', 
            bake_anim=False, 
            path_mode='AUTO', 
            embed_textures=False, 
            batch_mode='OFF', 
            use_batch_own_dir=True, 
            use_metadata=True, 
            axis_forward='-Z', 
            axis_up='Y')
        
        for obj in bpy.context.selected_objects:
            if obj.name == "Armature" and obj.type == 'ARMATURE':
                obj.name = skel_name

        self.report({'INFO'}, "Export completed.")

        return {'FINISHED'}

class MIRIWAKU_OP_export_accessories(Operator):
    bl_label = "Export Accessories"
    bl_idname = "miriwaku.export_accessories"
    
    @classmethod
    def poll(cls, context):
        return context.object is not None
    
    @classmethod
    def description(cls, context, properties):
        return 'Export selected body object into an FBX file'
    
    def execute(self, context):
        does_have_armature = False
        does_have_mesh = False
        does_have_eyes = False
        does_have_face = False
        does_have_hair = False
        does_have_accs = False
        does_have_armature_mod = False
        armature_count = 0
        skel_name = ''
        for obj in bpy.context.selected_objects:
            if obj.type == 'ARMATURE':
                if obj.name == 'Armature' or obj.name == 'armature':
                    self.report({'ERROR'}, "Object named Armature detected! The script will automatically handle the renaming, therefore no need for naming it Armature manually.")
                    return {'FINISHED'}
                does_have_armature = True
                armature_count = armature_count + 1
                if armature_count > 1:
                    self.report({'ERROR'}, "More than one armature object is selected! For simplicity, it's best to export just one armature.")
                    return {'FINISHED'}
                else:
                    skel_name = obj.name
            if obj.type == 'MESH':
                if 'eyes' in obj.name:
                    does_have_eyes = True
                if 'face' in obj.name:
                    does_have_face = True
                if 'hair' in obj.name:
                    does_have_hair = True
                if 'acc' in obj.name:
                    does_have_accs = True
                for modifier in obj.modifiers:
                    if modifier.type == "ARMATURE":
                        does_have_armature_mod = True
                        if obj.modifiers[0].object.name != skel_name:
                            self.report({'ERROR'}, "One or more of selected meshes are not tied to the selected armature!")
                            return {'FINISHED'}
                if not does_have_armature_mod:
                    self.report({'ERROR'}, "One or more of selected meshes are not bound to any armature!\nMake sure to check if they have the Armature modifier.")
                    return {'FINISHED'}
                        
                does_have_mesh = True
                
        if does_have_armature == False:
            self.report({'ERROR'}, "You must select an armature object!")
            return {'FINISHED'}
        if does_have_mesh == False:
            self.report({'ERROR'}, "At least a mesh object must be selected! Unreal Engine cannot import FBX containing just the armature.")
            return {'FINISHED'}
        if not does_have_accs:
            self.report({'ERROR'}, "Accessories mesh is not selected!")
            return {'FINISHED'}
        
        export_path = os.path.join(bpy.path.abspath('//'), 'SourceAsset', 'Accessories', bpy.context.object.users_collection[0].name)
        if not os.path.exists(export_path):
            os.makedirs(export_path)

        for obj in bpy.context.selected_objects:
            if obj.name == skel_name and obj.type == 'ARMATURE':
                obj.name = "Armature"

        file_export_path = os.path.join(export_path, "SK_%sAccessories.fbx" % (bpy.context.object.users_collection[0].name))

        bpy.ops.export_scene.fbx(
            filepath=file_export_path,
            check_existing=True, 
            filter_glob='*.fbx', 
            use_selection=True, 
            use_active_collection=False, 
            global_scale=1.0, 
            apply_unit_scale=True, 
            apply_scale_options='FBX_SCALE_NONE', 
            use_space_transform=True, 
            bake_space_transform=False, 
            object_types={'ARMATURE', 'MESH', 'OTHER'}, 
            use_mesh_modifiers=False, 
            mesh_smooth_type='FACE', 
            use_subsurf=False, 
            use_mesh_edges=False, 
            use_tspace=False, 
            use_custom_props=False, 
            add_leaf_bones=False, 
            primary_bone_axis='Y', 
            secondary_bone_axis='X', 
            use_armature_deform_only=True, 
            armature_nodetype='NULL', 
            bake_anim=False, 
            path_mode='AUTO', 
            embed_textures=False, 
            batch_mode='OFF', 
            use_batch_own_dir=True, 
            use_metadata=True, 
            axis_forward='-Z', 
            axis_up='Y')
        
        for obj in bpy.context.selected_objects:
            if obj.name == "Armature" and obj.type == 'ARMATURE':
                obj.name = skel_name

        self.report({'INFO'}, "Export completed.")

        return {'FINISHED'}



class MIRIWAKU_OP_rename_face_mt(Operator):
    bl_label = "Rename Face Shape Keys"
    bl_idname = "miriwaku.rename_face_mt"
    
    @classmethod
    def poll(cls, context):
        return context.object is not None
    
    @classmethod
    def description(cls, context, properties):
        return 'Rename selected face mesh\'s shape keys to accomodate for Control Rig in UE'
    
    def execute(self, context):
        does_have_armature = False
        does_have_mesh = False
        does_have_eyes = False
        does_have_face = False
        does_have_hair = False
        does_have_accs = False
        does_have_armature_mod = False
        armature_count = 0
        skel_name = ''
        changed_name = 0
        for obj in bpy.context.selected_objects:

            if 'face' in obj.name:
                does_have_face = True
            else:
                self.report({'ERROR'}, "Face object is not selected!")
                return {'FINISHED'}

            sknames = [
                ("M_a",         "Mouth_A"),
                ("M_i",         "Mouth_I"),
                ("M_u",         "Mouth_U"),
                ("M_e",         "Mouth_E"),
                ("M_o",         "Mouth_O"),
                ("M_n",         "Mouth_Flat"),
                ("M_egao",      "Mouth_WideOpen"),
                ("M_shinken",   "Mouth_Frown"),
                ("M_wide",      "Mouth_Grin"),
                ("M_up",        "Mouth_Up"),
                ("M_n2",        "Mouth_Doubt"),
                ("M_down",      "Mouth_Down"),
                ("M_odoroki",   "Mouth_Shocked"),
                ("M_narrow",    "Mouth_TinyGrin"),
                ("B_v_r",       "Brows_Angry_R"),
                ("B_v_l",       "Brows_Angry_L"),
                ("B_hati_r",    "Brows_Frown_R"),
                ("B_hati_l",    "Brows_Frown_L"),
                ("B_agari_r",   "Brows_Happy_R"),
                ("B_agari_l",   "Brows_Happy_L"),
                ("B_odoroki_r", "Brows_Doubt_R"),
                ("B_odoroki_l", "Brows_Doubt_L"),
                ("B_down",      "Brows_Down"),
                ("B_yori",      "Brows_Narrow"),
                ("E_metoji_r",  "Eyes_CloseDown_R"),
                ("E_metoji_l",  "Eyes_CloseDown_L"),
                ("E_wink_r",    "Eyes_CloseUp_R"),
                ("E_wink_l",    "Eyes_CloseUp_L"),
                ("E_open_r",    "Eyes_Shocked_R"),
                ("E_open_l",    "Eyes_Shocked_L")
            ]   
            
            
            for name, newname in sknames:
                # Get shape keys in the mesh
                keys = obj.data.shape_keys.key_blocks
                for key in keys:
                    # Validate key name before renaming it
                    if key.name == name:
                        changed_name += 1
                        key.name = key.name.replace(name, newname)
                    else:
                        continue
            
            if changed_name == 0:
                self.report({'WARNING'}, "No changes were made.")
            else:
                self.report({'INFO'}, "Shape key names changed.")

            return {'FINISHED'}



classes = [
    MIRIWAKU_PT_main_panel,
    MIRIWAKU_OP_fix_materials,
    MIRIWAKU_OP_remove_empties,
    MIRIWAKU_OP_export_outfit,
    MIRIWAKU_OP_export_head,
    MIRIWAKU_OP_export_accessories,
    MIRIWAKU_OP_rename_face_mt
]
 
 
 
def register():
    for cls in classes:
        bpy.utils.register_class(cls)
        
 
def unregister():
    for cls in classes:
        bpy.utils.unregister_class(cls)
 
 
 
if __name__ == "__main__":
    register()

