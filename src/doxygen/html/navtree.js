var NAVTREE =
[
  [ "Pixel'swar", "index.html", [
    [ "Related Pages", "pages.html", [
      [ "Todo List", "todo.html", null ]
    ] ],
    [ "Class List", "annotated.html", [
      [ "AssertException", "class_assert_exception.html", null ],
      [ "CameraManager", "class_camera_manager.html", null ],
      [ "ConsoleLogger", "class_console_logger.html", null ],
      [ "Exception", "class_exception.html", null ],
      [ "FileLogger", "class_file_logger.html", null ],
      [ "GraphicalEngine", "class_graphical_engine.html", null ],
      [ "GuiButtonItem", "class_gui_button_item.html", null ],
      [ "GuiItem", "class_gui_item.html", null ],
      [ "GuiManager", "class_gui_manager.html", null ],
      [ "Logger", "class_logger.html", null ],
      [ "PositionInformations", "struct_position_informations.html", null ],
      [ "SceneManager", "class_scene_manager.html", null ],
      [ "SceneNode", "class_scene_node.html", null ],
      [ "SceneNodeItem", "class_scene_node_item.html", null ],
      [ "SceneNodeShapeItem", "class_scene_node_shape_item.html", null ],
      [ "SceneNodeSpriteItem", "class_scene_node_sprite_item.html", null ],
      [ "Singleton< T >", "class_singleton.html", null ],
      [ "testClass", "classtest_class.html", null ],
      [ "TextureManager", "class_texture_manager.html", null ]
    ] ],
    [ "Class Index", "classes.html", null ],
    [ "Class Hierarchy", "hierarchy.html", [
      [ "CameraManager", "class_camera_manager.html", null ],
      [ "Exception", "class_exception.html", [
        [ "AssertException", "class_assert_exception.html", null ]
      ] ],
      [ "Logger", "class_logger.html", [
        [ "ConsoleLogger", "class_console_logger.html", null ],
        [ "FileLogger", "class_file_logger.html", null ]
      ] ],
      [ "PositionInformations", "struct_position_informations.html", null ],
      [ "SceneManager", "class_scene_manager.html", [
        [ "GuiManager", "class_gui_manager.html", null ]
      ] ],
      [ "SceneNode", "class_scene_node.html", null ],
      [ "SceneNodeItem", "class_scene_node_item.html", [
        [ "GuiItem", "class_gui_item.html", [
          [ "GuiButtonItem", "class_gui_button_item.html", null ]
        ] ],
        [ "SceneNodeShapeItem", "class_scene_node_shape_item.html", null ],
        [ "SceneNodeSpriteItem", "class_scene_node_sprite_item.html", null ]
      ] ],
      [ "Singleton< T >", "class_singleton.html", null ],
      [ "Singleton< GraphicalEngine >", "class_singleton.html", [
        [ "GraphicalEngine", "class_graphical_engine.html", null ]
      ] ],
      [ "Singleton< TextureManager >", "class_singleton.html", [
        [ "TextureManager", "class_texture_manager.html", null ]
      ] ],
      [ "testClass", "classtest_class.html", null ]
    ] ],
    [ "Class Members", "functions.html", null ],
    [ "File List", "files.html", [
      [ "cameramanager.h", null, null ],
      [ "consolelogger.h", null, null ],
      [ "exception.h", null, null ],
      [ "filelogger.h", null, null ],
      [ "graphicalengine.h", null, null ],
      [ "guibuttonitem.h", null, null ],
      [ "guiitem.h", null, null ],
      [ "guimanager.h", null, null ],
      [ "logger.h", null, null ],
      [ "positioninformations.h", null, null ],
      [ "scenemanager.h", null, null ],
      [ "scenenode.h", null, null ],
      [ "scenenodeitem.h", null, null ],
      [ "scenenodeshapeitem.h", null, null ],
      [ "scenenodespriteitem.h", null, null ],
      [ "singleton.h", null, null ],
      [ "texturemanager.h", null, null ]
    ] ]
  ] ]
];

function createIndent(o,domNode,node,level)
{
  if (node.parentNode && node.parentNode.parentNode)
  {
    createIndent(o,domNode,node.parentNode,level+1);
  }
  var imgNode = document.createElement("img");
  if (level==0 && node.childrenData)
  {
    node.plus_img = imgNode;
    node.expandToggle = document.createElement("a");
    node.expandToggle.href = "javascript:void(0)";
    node.expandToggle.onclick = function() 
    {
      if (node.expanded) 
      {
        $(node.getChildrenUL()).slideUp("fast");
        if (node.isLast)
        {
          node.plus_img.src = node.relpath+"ftv2plastnode.png";
        }
        else
        {
          node.plus_img.src = node.relpath+"ftv2pnode.png";
        }
        node.expanded = false;
      } 
      else 
      {
        expandNode(o, node, false);
      }
    }
    node.expandToggle.appendChild(imgNode);
    domNode.appendChild(node.expandToggle);
  }
  else
  {
    domNode.appendChild(imgNode);
  }
  if (level==0)
  {
    if (node.isLast)
    {
      if (node.childrenData)
      {
        imgNode.src = node.relpath+"ftv2plastnode.png";
      }
      else
      {
        imgNode.src = node.relpath+"ftv2lastnode.png";
        domNode.appendChild(imgNode);
      }
    }
    else
    {
      if (node.childrenData)
      {
        imgNode.src = node.relpath+"ftv2pnode.png";
      }
      else
      {
        imgNode.src = node.relpath+"ftv2node.png";
        domNode.appendChild(imgNode);
      }
    }
  }
  else
  {
    if (node.isLast)
    {
      imgNode.src = node.relpath+"ftv2blank.png";
    }
    else
    {
      imgNode.src = node.relpath+"ftv2vertline.png";
    }
  }
  imgNode.border = "0";
}

function newNode(o, po, text, link, childrenData, lastNode)
{
  var node = new Object();
  node.children = Array();
  node.childrenData = childrenData;
  node.depth = po.depth + 1;
  node.relpath = po.relpath;
  node.isLast = lastNode;

  node.li = document.createElement("li");
  po.getChildrenUL().appendChild(node.li);
  node.parentNode = po;

  node.itemDiv = document.createElement("div");
  node.itemDiv.className = "item";

  node.labelSpan = document.createElement("span");
  node.labelSpan.className = "label";

  createIndent(o,node.itemDiv,node,0);
  node.itemDiv.appendChild(node.labelSpan);
  node.li.appendChild(node.itemDiv);

  var a = document.createElement("a");
  node.labelSpan.appendChild(a);
  node.label = document.createTextNode(text);
  a.appendChild(node.label);
  if (link) 
  {
    a.href = node.relpath+link;
  } 
  else 
  {
    if (childrenData != null) 
    {
      a.className = "nolink";
      a.href = "javascript:void(0)";
      a.onclick = node.expandToggle.onclick;
      node.expanded = false;
    }
  }

  node.childrenUL = null;
  node.getChildrenUL = function() 
  {
    if (!node.childrenUL) 
    {
      node.childrenUL = document.createElement("ul");
      node.childrenUL.className = "children_ul";
      node.childrenUL.style.display = "none";
      node.li.appendChild(node.childrenUL);
    }
    return node.childrenUL;
  };

  return node;
}

function showRoot()
{
  var headerHeight = $("#top").height();
  var footerHeight = $("#nav-path").height();
  var windowHeight = $(window).height() - headerHeight - footerHeight;
  navtree.scrollTo('#selected',0,{offset:-windowHeight/2});
}

function expandNode(o, node, imm)
{
  if (node.childrenData && !node.expanded) 
  {
    if (!node.childrenVisited) 
    {
      getNode(o, node);
    }
    if (imm)
    {
      $(node.getChildrenUL()).show();
    } 
    else 
    {
      $(node.getChildrenUL()).slideDown("fast",showRoot);
    }
    if (node.isLast)
    {
      node.plus_img.src = node.relpath+"ftv2mlastnode.png";
    }
    else
    {
      node.plus_img.src = node.relpath+"ftv2mnode.png";
    }
    node.expanded = true;
  }
}

function getNode(o, po)
{
  po.childrenVisited = true;
  var l = po.childrenData.length-1;
  for (var i in po.childrenData) 
  {
    var nodeData = po.childrenData[i];
    po.children[i] = newNode(o, po, nodeData[0], nodeData[1], nodeData[2],
        i==l);
  }
}

function findNavTreePage(url, data)
{
  var nodes = data;
  var result = null;
  for (var i in nodes) 
  {
    var d = nodes[i];
    if (d[1] == url) 
    {
      return new Array(i);
    }
    else if (d[2] != null) // array of children
    {
      result = findNavTreePage(url, d[2]);
      if (result != null) 
      {
        return (new Array(i).concat(result));
      }
    }
  }
  return null;
}

function initNavTree(toroot,relpath)
{
  var o = new Object();
  o.toroot = toroot;
  o.node = new Object();
  o.node.li = document.getElementById("nav-tree-contents");
  o.node.childrenData = NAVTREE;
  o.node.children = new Array();
  o.node.childrenUL = document.createElement("ul");
  o.node.getChildrenUL = function() { return o.node.childrenUL; };
  o.node.li.appendChild(o.node.childrenUL);
  o.node.depth = 0;
  o.node.relpath = relpath;

  getNode(o, o.node);

  o.breadcrumbs = findNavTreePage(toroot, NAVTREE);
  if (o.breadcrumbs == null)
  {
    o.breadcrumbs = findNavTreePage("index.html",NAVTREE);
  }
  if (o.breadcrumbs != null && o.breadcrumbs.length>0)
  {
    var p = o.node;
    for (var i in o.breadcrumbs) 
    {
      var j = o.breadcrumbs[i];
      p = p.children[j];
      expandNode(o,p,true);
    }
    p.itemDiv.className = p.itemDiv.className + " selected";
    p.itemDiv.id = "selected";
    $(window).load(showRoot);
  }
}

